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
 * @file    Solvers.hpp
 * @author  Marvin Smith
 * @date    10/15/2023
 */
#pragma once

// Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/math/matrix/MatrixN.hpp>
#include <terminus/math/types/Type_Deduction.hpp>
#include <terminus/math/vector/VectorN.hpp>

namespace tmns::math::linalg {

Result<VectorN<double>> solve_symmetric( const MatrixN<double>& A,
                                         const VectorN<double>& b );

/**
 * Solve the equation Ax=b where A is a symmetric positive definite matrix.  This version of
 * this method will not modify A and b. The result (x) is returned as the return value.
 */
template <typename AMatrixT,
          typename BVectorT>
Result<VectorN<typename Promote_Type<typename AMatrixT::value_type,
                                     typename BVectorT::value_type>::type>>
    solve_symmetric( const AMatrixT& A,
                     const BVectorT& B )
{
    using real_type = typename Promote_Type<typename AMatrixT::value_type,
                                            typename BVectorT::value_type>::type;
    MatrixN<real_type> Abuf = A;
    VectorN<real_type> result = B;

    return solve_symmetric( A, B );
}

/**
 * x = solve(A,b) - Computes the solution to a real system of linear equations:
 *
 * A*x=b
 *
 * Based on the LAPACK GESV routines, this solution is computed using LU decomposition and
 * back/forward substitution.
 *
 * Computes the minimum-norm solution to a real linear least squares problem.
 */
Result<VectorN<double>> solve( const MatrixN<double>& mat_A,
                               const VectorN<double>& vec_b,
                               double                 eps = 0.00000001 );

} // End of tmns::math::linalg