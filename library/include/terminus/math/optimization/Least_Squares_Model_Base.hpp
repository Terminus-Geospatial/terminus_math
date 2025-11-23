/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/*                                                                                    */
/*                           Copyright (c) 2024 Terminus LLC                          */
/*                                                                                    */
/*                                All Rights Reserved.                                */
/*                                                                                    */
/*          Use of this source code is governed by LICENSE in the repo root.          */
/*                                                                                    */
/***************************# INTELLECTUAL PROPERTY RIGHTS ****************************/
/**
 * @file    least_squares_model_base.hpp
 * @author  Marvin Smith
 * @date    10/14/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/math/matrix.hpp>
#include <terminus/math/optimization/lm_enums.hpp>
#include <terminus/math/vector/vectorn.hpp>

namespace tmns::math::optimize {

/**
 * First thing we need is a generic idea of a measurement function or model function.
 * The model function needs to provide a way to evaluate h(x) as well as a way to
 * differentiate h() to get its Jacobian.  In the past we've implemented a fallback
 * option by computing the Jacobian numerically which seems to work pretty well.  This
 * Least_Squares_Model_Function should be implemented as an inherited class using the
 * curiously recurring template mechanism (CRTP) using the following syntax:
 *
 *  class MyLeastSquaresModel : public LeastSquaresModelBase<MyLeastSquaresModel> {
 *  ...
 *
 *  You will (at the very least) need to provide the following things in your base class:
 *
 *  Defines a result_type that is the type returned by evaluating the functor.  Typically
 * VectorN<float> or VectorN<double>
 *
 * - Defines a domain_type that is the type of the search space.  Often a VectorN<foo>, but
 * can reflect other topologies if needed.
 *
 * - Defines a jacobian_type corresponding to the space of jacobian matrices.  Typically
 *   Matrix<foo>.
 *
 * - Defines a method: result_type operator()( domain_type const& x ) const;
 *   that evaluates the model function at the given point.
 *
 * - The domain_type must implement a method: domain_type domain_type::operator+( gradient_type const& g ) const;
 *   that adds a tangent vector to a position.  You get this for free if both domain_type and
 *   gradient_type are Vector<foo>.  This is where you do most of the hard work if domain_type
 *   represents some non-trivial topological space.
 *
 * - The result_type must implement a method: double result_type::norm_2( result_type const& g ) const;
 *   that is used in some optimizers to compute the error.  You get this for free if
 *   result_type is a Vector<foo>.
 *
 * - The jacobian_type must implement several matrix-like methods such as scalar multiplication
 *   on the left.  You get this for free in the usual case when jacobian_type is just Matrix<foo>.
 *
 * - In addition, depending on the application, you may want to also define:
 *
 *     - Defines a method: jacobian_type jacobian( domain_type const& x ) const;
 *       that evaluates the jacobian of the model function at the given point.  This will
 *       override the default implementation in this base class which computes the derivatives
 *       numerically.
 */
template <typename ImplT>
class Least_Squares_Model_Base
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

        /**
         * This default implementation evaluates dh(x;p)/dx at the supplied location
         * using numerical differentiation.  It highly recommended that you override
         * this method in your sub-class and provide an exact computation of the gradient.
         *
         * Note that this numerical derivative assumes that the result_type and jacobian_type
         * types behave like vector and matrix objects, respectively.
        */
        template <typename DomainT>
        MatrixN<double> jacobian( const DomainT& x ) const
        {
            tmns::log::trace( ADD_CURRENT_LOC(),
                              "Start of Method ",
                              x.to_log_string() );
            // Get nominal function value
            auto h0 = impl().operator()(x);

            // Jacobian is #params x #outputs
            MatrixN<double> H( h0.size(), x.size() );

            /**
             * For each param dimension, add epsilon and re-evaluate h() to
             * get numerical derivative w.r.t. that parameter
             */
            for( unsigned i=0; i<x.size(); ++i )
            {
                DomainT xi = x;

                // Variable step size, depending on parameter value
                double epsilon = 1e-7 + std::fabs( xi(i) * 1e-7 );
                xi(i) += epsilon;

                // Evaluate function with this step and compute the derivative w.r.t. parameter i
                VectorN<double> hi = impl().operator()(xi);
                select_col(H,i) = this->difference( hi, h0 ) / epsilon;
            }
            return H;
        }

        /**
         * The utility of a differencing function is that it allows looping topologies
         * like angles to be handled without putting the logic in the L-M implementation.
         * You probably won't need to override this method in your base class implementation
         * unless you are working with unusual result_space topologies.
         */
        template <class T>
        T difference( const T& a,
                      const T& b ) const
        {
            return ( a - b );
        }

}; // End of Least_Squares_Model_Base

} // End of tmns::math::optimize