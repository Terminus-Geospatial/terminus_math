/**
 * @file    Operations.hpp
 * @author  Marvin Smith
 * @date    11/12/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/math/linalg/SVD.hpp>
#include <terminus/math/matrix.hpp>
#include <terminus/math/vector.hpp>

namespace tmns::math::linalg {

/**
 * Solve for nullity .. using previous SVD results
 */
template <typename MatrixT,
          typename MatrixT2,
          typename VectorT>
size_t nullity( const Matrix_Base<MatrixT>&  A,
                const Matrix_Base<MatrixT2>& U,
                const Vector_Base<VectorT>&  S,
                const Matrix_Base<MatrixT2>& V,
                typename VectorT::value_type thresh = -1 )
{
    using value_type = typename MatrixT::value_type;

    value_type th = ( thresh >= 0. ? thresh : detail::calc_threshold(A, S) );
    size_t nn = boost::numeric_cast<size_t>( A.impl().cols() - S.impl().size() );
    for( size_t j = 0; j < S.impl().size(); j++ )
    {
        if ( S.impl()[j] <= th )
        {
            nn++;
        }
    }
    return nn;
}

/**
 * Nullity of a matrix (again from Numerical Receipes)
 */
template <typename MatrixT>
size_t nullity( const Matrix_Base<MatrixT>& A,
                typename MatrixT::value_type thresh = -1 )
{
    using value_type = typename MatrixT::value_type;
    MatrixN<value_type> U, V;
    VectorN<value_type> S;
    
    auto res = complete_svd( A.impl(), U, S, V );
    V = transpose( V );
    return nullity( A, U, S, V, thresh );
}

/**
 * Solve for the nullspace of a Matrix A. If Ax = [0], the nullspace is an x that is not zero.
 */
template <typename MatrixT>
ImageResult<MatrixN<typename MatrixT::value_type>> nullspace( const Matrix_Base<MatrixT>&  A,
                                                              typename MatrixT::value_type thresh = -1 )
{
    typedef typename MatrixT::value_type value_type;
    MatrixN<value_type> U, V;
    VectorN<value_type> S;
    auto svd_res = complete_svd( A.impl(), U, S, V );
    V = transpose(V);

    size_t nty = nullity( A.impl(), U, S, V, thresh );
    if ( nty == 0 )
    {
        return MatrixN<value_type>( 0, 0 );
    }
    Matrix<value_type> nullsp(A.impl().cols(), nty );
    value_type th = ( thresh >= 0. ? thresh : detail::calc_threshold(A, S) );
    size_t nn = 0;
    for ( size_t j = 0; j < A.impl().cols(); j++ ) {
      if ( j < S.size() )
        if ( S[j] > th )
          continue;
      for ( size_t jj = 0; jj < A.impl().cols(); jj++ )
        nullsp(jj,nn) = V[jj][j];
      nn++;
    }
    return nullsp;
  }


} // End of tmns::math::linalg namespace