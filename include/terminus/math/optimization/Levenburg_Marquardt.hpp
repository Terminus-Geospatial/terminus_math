/**
 * @file    Levenburg_Marquardt.hpp
 * @author  Marvin Smith
 * @date    10/14/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/math/linalg/Solvers.hpp>
#include <terminus/math/matrix/Matrix_Operations.hpp>
#include <terminus/math/optimization/Least_Squares_Model_Base.hpp>
#include <terminus/math/optimization/LM_Enums.hpp>

namespace tmns::math::optimize {

/**
 * Levenberg-Marquardt is an algorithm for solving problems of the form
 * 
 * J(p) = sum_i ( z_i - h(x_i) )^2
 * 
 * That is, a least squares problem where the objective is to find a parameter vector
 * x such that the model function h(x), evaluates as closely as possible to the
 * observations z in a least squares sense, that is, the cost function J(p) is minimized.
 * 
 * Requires:
 * - a seed parameter vector x
 * - an observation vector z
 * - a data model derived from LeastSquaresModelBase, which includes
 *      - the model function,
 *      - its Jacobian
 * 
 * The cost function in L-M is always the inner product of the difference between an
 * observation and the expected observation given the model parameters.  This means
 * we can compute the cost function and its derivatives if we know the measurement
 * function and its derivatives.
 */

#define MATH_LM_ABS_TOL (1e-16)
#define MATH_LM_REL_TOL (1e-16)
#define MATH_LM_MAX_ITER (100)

template <typename ImplT>
ImageResult<typename ImplT::domain_type> levenberg_marquardt( const Least_Squares_Model_Base<ImplT>& least_squares_model,
                                                              const typename ImplT::domain_type&     seed,
                                                              const typename ImplT::result_type&     observation,
                                                              LM_STATUS_CODE&                        status,
                                                              double                                 abs_tolerance  = MATH_LM_ABS_TOL,
                                                              double                                 rel_tolerance  = MATH_LM_REL_TOL,
                                                              double                                 max_iterations = MATH_LM_MAX_ITER)
{
    // Initialize the status
    status = LM_STATUS_CODE::ERROR_DID_NOT_CONVERGE;

    const ImplT& model = least_squares_model.impl();
    bool   done   = false;
    double Rinv   = 10;
    double lambda = 0.1;

    typename ImplT::domain_type x_try, x = seed;
    typename ImplT::result_type h        = model(x);
    typename ImplT::result_type error    = model.difference(observation, h);
    double norm_start = error.magnitude();

    tmns::log::debug( "LM: initial guess for the model is ", seed.to_log_string() );
    tmns::log::trace( "LM: starting error ", error.to_log_string() );
    tmns::log::debug( "LM: starting norm is: ", norm_start );

    // Solution may already be good enough
    if( norm_start < abs_tolerance )
    {
        status = LM_STATUS_CODE::ERROR_CONVERGED_ABS_TOLERANCE;
        tmns::log::debug( "CONVERGED TO ABSOLUTE TOLERANCE" );
        done = true;
    }

    int outer_iter = 0;
    while( !done )
    {
        bool shortCircuit = false;
        outer_iter++;
        tmns::log::debug( "LM: outer iteration ", outer_iter, "   x = ", x.to_log_string() );

        // Compute the value, derivative, and hessian of the cost function
        // at the current point.  These remain valid until the parameter
        // vector changes.

        // expected measurement with new x
        h = model(x);

        // Difference between observed and predicted and error (2-norm of difference)
        error = model.difference( observation, h );
        norm_start = error.magnitude();
        tmns::log::debug( ADD_CURRENT_LOC(), 
                          "LM: outer iteration starting robust norm: ",
                          norm_start );

        // @todo:  Fix matrix multiplication problem.

        // Measurement Jacobian
        typename ImplT::jacobian_type J = model.jacobian(x);

        MatrixN<double> JT = transpose( J );
        VectorN<double> JTE = (JT * error);
        VectorN<double> RJ = Rinv * JTE;
        VectorN<double> del_J = -1.0 * RJ;

        // Hessian of cost function (using Gauss-Newton approximation)
        MatrixN<double> TJ = transpose( J );
        MatrixN<double> TJJ = TJ * J;

        MatrixN<double> hessian = Rinv * TJJ;
        tmns::log::trace( ADD_CURRENT_LOC(), "hessian: ", hessian.to_log_string() );

        int64_t iterations = 0;
        double norm_try = norm_start + 1.0;
        while( norm_try > norm_start )
        {
            tmns::log::trace( ADD_CURRENT_LOC(), "Norm Try: ", norm_try, ", Norm Start: ", norm_start );
            // Increase diagonal elements to dynamically mix gradient
            // descent and Gauss-Newton.
            Matrix<double> hessian_lm = hessian;
            for( unsigned i = 0; i < hessian_lm.rows(); ++i )
            {
                hessian_lm(i,i) += hessian_lm(i,i)*lambda + lambda;
            }

            // Solve for update
            typename ImplT::domain_type delta_x;
            if( hessian_lm.rows() <= 2 && hessian_lm.determinant() > 0.0 )
            {
                // Direct method is more efficient for small matrices, also
                // here we avoid calling LAPACK which we've seen misbehave
                // in this situation in a multi-threaded environment.
                delta_x = hessian_lm.inverse() * del_J;
            }
            else
            {
                try
                {
                    // By construction, hessian_lm is symmetric and positive-definite.
                    auto solve_res = linalg::solve_symmetric( hessian_lm, del_J );
                    delta_x = solve_res.value();
                }
                catch ( const std::exception& e )
                {
                    // If lambda is very small, the matrix becomes numerically
                    // singular. In that case use the more general least_squares solver.
                    auto solve_res = linalg::solve( hessian_lm, del_J );
                    if( solve_res.has_error() )
                    {
                        return solve_res.error();
                    }
                    delta_x = solve_res.value();
                }
            }

            // update parameter vector
            x_try = x - delta_x;

            typename ImplT::result_type h_try = model(x_try);

            typename ImplT::result_type error_try = model.difference(observation, h_try);
            norm_try = error_try.magnitude();

            tmns::log::debug( ADD_CURRENT_LOC(), "LM: inner iteration ", iterations, " error is ", error_try.to_log_string() );
            tmns::log::trace( ADD_CURRENT_LOC(), "\tLM: inner iteration ", iterations, " norm is ", norm_try );

            if( norm_try > norm_start )
            {
                // Increase lambda and try again
                lambda *= 10;
            }

            ++iterations; // Sanity check on iterations in this loop
            if( iterations > 5 )
            {
                tmns::log::debug( "\n****LM: too many inner iterations - short circuiting" );
                shortCircuit = true;
                norm_try     = norm_start;
            }
            tmns::log::debug( "\tlambda = ", lambda );
        }

        // Percentage change convergence criterion. Only if we did not do a short-circuit,
        // as in that case the solution did not improve.
        if( !shortCircuit && ( ( norm_start - norm_try ) / norm_start ) < rel_tolerance )
        {
            status = LM_STATUS_CODE::ERROR_CONVERGED_REL_TOLERANCE;
            tmns::log::debug( "CONVERGED TO RELATIVE TOLERANCE" );
            done = true;
        }

        // Absolute error convergence criterion
        if( norm_try < abs_tolerance )
        {
            status = LM_STATUS_CODE::ERROR_CONVERGED_ABS_TOLERANCE;
            tmns::log::debug( "CONVERGED TO ABSOLUTE TOLERANCE" );
            done = true;
        }

        // Max iterations convergence criterion
        if( outer_iter >= max_iterations )
        {
            tmns::log::debug( "REACHED MAX ITERATIONS!" );
            done = true;
        }

        // Take trial parameters as new parameters
        // If we short-circuited the inner loop, then we didn't actually find a
        // better p, so don't update it.
        if( !shortCircuit )
        {
            x = x_try;
        }

        // Take trial error as new error
        norm_start = norm_try;

        // Decrease lambda
        lambda /= 10;
        tmns::log::debug( "lambda = ", lambda, "\nLM: end of outer iteration ",
                          outer_iter, " with error ", norm_try );
    }
    tmns::log::debug( "LM: finished with: ", outer_iter );
    return x;
} // End levenberg_marquardt


/**
 * As the similar class above, but with fixed matrix sizes and no logging.
 */
template <typename ImplT,
          int      NI,
          int      NO>
class Least_Squares_Model_Base_Fixed
{
    public:

        /**
         * Access the underlying type
         */
        ImplT&  impl()
        {
            return static_cast<ImplT&>( *this );
        }

        /**
         * Access the underlying type
         */
        ImplT const& impl() const
        {
            return static_cast<ImplT const&>( *this );
        }

        template <class DomainT>
        Matrix<double, NO, NI> jacobian( DomainT const& x ) const
        {
            // Get nominal function value
            //std::cout << "x; = " << x << std::endl;
            //std::cout << "impl().operator()(x); = " << impl().operator()(x) << std::endl;
            Vector_<double, NO> h0 = impl().operator()(x);
            //Vector<double> h0 = impl().operator()(x);
            //std::cout << "h0 = " << h0 << std::endl;

            // Jacobian is #params x #outputs
            Matrix<double, NO, NI> H;
            //std::cout << "H = " << H << std::endl;

            // For each param dimension, add epsilon and re-evaluate h() to
            // get numerical derivative w.r.t. that parameter
            Vector_<double, NO> hi;
            for( unsigned i = 0; i < x.size(); ++i )
            {
                DomainT xi = x;

                // Variable step size, depending on parameter value
                double epsilon = 1e-7 + fabs(xi(i)*1e-7);
                xi(i) += epsilon;

                // Evaluate function with this step and compute the derivative w.r.t. parameter i
                hi = impl().operator()(xi);
                //Vector<double> hi = impl().operator()(xi);
                //std::cout << "hi = " << hi << std::endl;
                select_col(H,i) = this->difference(hi,h0)/epsilon;
            }
            //std::cout << "H = " << H << std::endl;
            return H;
        }

        template <class T>
        T difference( const T& a,
                      const T& b ) const
        {
            return ( a - b );
        }
}; // End LeastSquaresModelBaseSQ


/**
 * As the similar function above, but with a fixed, square matrix size.
 */
template <typename ImplT,
          int      NI,
          int      NO>
ImageResult<typename ImplT::domain_type> levenberg_marquardt_fixed( const Least_Squares_Model_Base_Fixed<ImplT, NI, NO>& least_squares_model,
                                                                    const typename ImplT::domain_type&                   seed,
                                                                    const typename ImplT::result_type&                   observation,
                                                                    LM_STATUS_CODE&                                      status,
                                                                    double abs_tolerance = MATH_LM_ABS_TOL,
                                                                    double rel_tolerance = MATH_LM_REL_TOL,
                                                                    double max_iterations = MATH_LM_MAX_ITER) {

    status = LM_STATUS_CODE::ERROR_DID_NOT_CONVERGE;

    const ImplT& model = least_squares_model.impl();
    bool   done   = false;
    double Rinv   = 10;
    double lambda = 0.1;

    typename ImplT::domain_type x_try, x = seed;
    typename ImplT::result_type h = model(x);
    typename ImplT::result_type error = model.difference(observation, h);
    double norm_start = error.magnitude();

    tmns::log::debug( "LM: initial guess for the model is ", seed.to_log_string() );
    tmns::log::debug( "LM: starting error ", error.to_log_string() );
    tmns::log::debug( "LM: starting norm is: ", norm_start );

    // Solution may already be good enough
    if( norm_start < abs_tolerance )
    {
        status = LM_STATUS_CODE::ERROR_CONVERGED_ABS_TOLERANCE;
        tmns::log::debug( "CONVERGED TO ABSOLUTE TOLERANCE" );
        done = true;
    }

    typename ImplT::jacobian_type J;
    Matrix<double, NI, NO> J_trans;
    Matrix<double, NI, NI> J_trans_J;
    Vector_<double, NI> del_J;
    Matrix<double, NI, NI> hessian, hessian_lm, hessian_lm_inv;
    
    tmns::log::debug( "Start of loops" );
    int outer_iter = 0;
    while( !done )
    {
        bool shortCircuit = false;
        outer_iter++;
        tmns::log::debug( "LM: outer iteration ", outer_iter, "   x = ", x.to_log_string() );

        // Compute the value, derivative, and hessian of the cost function
        // at the current point.  These remain valid until the parameter
        // vector changes.

        // expected measurement with new x
        h = model(x);

        // Difference between observed and predicted and error (2-norm of difference)
        error = model.difference( observation, h );
        norm_start = error.magnitude();
        tmns::log::debug( ADD_CURRENT_LOC(), 
                          "LM: outer iteration starting robust norm: ",
                          norm_start );

        // Measurement Jacobian
        J = model.jacobian(x);

        tmns::log::trace( "J: ", J.to_log_string() );
      
        J_trans = transpose( J );
        tmns::log::trace( "J Transpose: ", J_trans.to_log_string() );

        del_J = -1.0 * Rinv * ( J_trans * error );
        //Vector<double> del_J = -1.0 * Rinv * (J_trans * error);
        //std::cout << "del_J = " << del_J << std::endl;

        // Hessian of cost function (using Gauss-Newton approximation)
        J_trans_J = J_trans * J;
        //std::cout << "J_trans_J = " << J_trans_J << std::endl;
        hessian = Rinv * J_trans_J;
        //Matrix<double> hessian = Rinv * J_trans_J;

        //std::cout << "hessian = " << hessian << std::endl;
      
        int iterations = 0;
        double norm_try = norm_start+1.0;
        while (norm_try > norm_start)
        {

            // Increase diagonal elements to dynamically mix gradient
            // descent and Gauss-Newton.
            hessian_lm = hessian;
            //Matrix<double> hessian_lm = hessian;
            for( unsigned i = 0; i < hessian_lm.rows(); ++i )
            {
                hessian_lm(i,i) += hessian_lm(i,i)*lambda + lambda;
            }

            // Solve for update
            typename ImplT::domain_type delta_x;
            if( hessian_lm.rows() <= 2 && hessian_lm.determinant() > 0.0 )
            {
                // Direct method is more efficient for small matrices, also
                // here we avoid calling LAPACK which we've seen misbehave
                // in this situation in a multi-threaded environment.
                delta_x = hessian_lm.inverse() * del_J;
            }
            else
            {
                try
                {
                    // By construction, hessian_lm is symmetric and positive-definite.
                    auto solve_res = linalg::solve_symmetric( hessian_lm, del_J );
                    delta_x = solve_res.value();
                }
                catch( const std::exception& e )
                {
                    // If lambda is very small, the matrix becomes numerically
                    // singular. In that case use the more general least_squares solver.
                    auto solve_res = linalg::solve( hessian_lm, del_J );
                    if( solve_res.has_error() )
                    {
                        return outcome::fail( (core::error::ErrorCode)(solve_res.error().code().value()),
                                              solve_res.error().message() );
                    }
                    delta_x = solve_res.value();
                }
            }

            // update parameter vector
            x_try = x - delta_x;

            typename ImplT::result_type h_try = model(x_try);

            typename ImplT::result_type error_try = model.difference(observation, h_try);
            norm_try = error_try.magnitude();

            tmns::log::debug( ADD_CURRENT_LOC(), "LM: inner iteration ", iterations, " error is ", error_try.to_log_string() );
            tmns::log::trace( ADD_CURRENT_LOC(), "\tLM: inner iteration ", iterations, " norm is ", norm_try );

            if( norm_try > norm_start )
            {
                // Increase lambda and try again
                lambda *= 10;
            }

            ++iterations; // Sanity check on iterations in this loop
        
            if( iterations > 5 )
            {
                tmns::log::debug( "\n****LM: too many inner iterations - short circuiting" );
                shortCircuit = true;
                norm_try     = norm_start;
            }
            tmns::log::debug( "\tlambda = ", lambda );
        }

        // Percentage change convergence criterion. Only if we did not do a short-circuit,
        // as in that case the solution did not improve.
        if( !shortCircuit && ( ( norm_start-norm_try ) / norm_start ) < rel_tolerance )
        {
            status = LM_STATUS_CODE::ERROR_CONVERGED_REL_TOLERANCE;
            tmns::log::debug( "CONVERGED TO RELATIVE TOLERANCE" );
            done = true;
        }

        // Absolute error convergence criterion
        if( norm_try < abs_tolerance )
        {
            status = LM_STATUS_CODE::ERROR_CONVERGED_ABS_TOLERANCE;
            tmns::log::debug( "CONVERGED TO ABSOLUTE TOLERANCE" );
            done = true;
        }

        // Max iterations convergence criterion
        if( outer_iter >= max_iterations )
        {
            tmns::log::debug( "REACHED MAX ITERATIONS!" );
            done = true;
        }

        // Take trial parameters as new parameters
        // If we short-circuited the inner loop, then we didn't actually find a
        // better p, so don't update it.
        if( !shortCircuit )
        {
            x = x_try;
        }

        // Take trial error as new error
        norm_start = norm_try;

        // Decrease lambda
        lambda /= 10;
        tmns::log::debug( "lambda = ", lambda );
        tmns::log::debug( "LM: end of outer iteration ", outer_iter, " with error ", norm_try );
    }

    tmns::log::debug( "LM: finished with: ", outer_iter );
    return outcome::ok<typename ImplT::domain_type>( x );

} // End levenberg_marquardt

} // End of tmns::math::optimize