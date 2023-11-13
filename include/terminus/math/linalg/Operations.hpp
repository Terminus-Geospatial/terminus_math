/**
 * @file    Operations.hpp
 * @author  Marvin Smith
 * @date    11/12/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/math/matrix.hpp>
#include <terminus/math/vector.hpp>

namespace tmns::math::linalg {

namespace detail {
template <typename MatrixT,
          typename VectorT>
typename VectorT::value_type calc_threshold( const Matrix_Base<MatrixT>& A,
                                             const Vector_Base<VectorT>& S )
{
      return typename VectorT::value_type( 0.5
                * sqrt(double(A.impl().cols()) + double(A.impl().rows()) + 1.)
                * S.impl()[0]
                * std::numeric_limits<typename VectorT::value_type>::epsilon() );
}
} // End of detail namespace

/**
 * Solve for nullity .. using previous SVD results
 */
template <typename MatrixT,
          typename MatrixT2,
          typename VectorT>
size_t nullity( const Matrix_Base<MatrixT>&  A,
                const Matrix_Base<MatrixT2>& U,
                const Vector_Base<VectorT>& S,
                const MatrixBase<MatrixT2>& V,
                typename VectorT::value_type thresh = -1 )
{
    using value_type = typename MatrixT::value_type;

    value_type th = ( thresh >= 0. ? thresh : detail::calc_threshold(A, S) );
    size_t nn = boost::numeric_cast<size_t>(A.impl().cols()-S.impl().size());
    for ( size_t j = 0; j < S.impl().size(); j++ ) {
      if ( S.impl()[j] <= th )
        nn++;
    }
    return nn;
  }

  // Nullity of a matrix (again from Numerical Receipes)
  template <class MatrixT>
  inline size_t nullity( MatrixBase<MatrixT> const& A,
                         typename MatrixT::value_type thresh = -1 ) {
    typedef typename MatrixT::value_type value_type;
    Matrix<value_type> U, V;
    Vector<value_type> S;
    complete_svd( A.impl(), U, S, V );
    V = transpose(V);
    return nullity(A,U,S,V,thresh);
  }

  // Solve for the nullspace of a Matrix A. If Ax = [0], the nullspace
  // is an x that is not zero.
  template <class MatrixT >
  inline Matrix<typename MatrixT::value_type> nullspace( MatrixBase<MatrixT> const& A,
                                                         typename MatrixT::value_type thresh = -1 ) {
    typedef typename MatrixT::value_type value_type;
    Matrix<value_type> U, V;
    Vector<value_type> S;
    complete_svd( A.impl(), U, S, V );
    V = transpose(V);

    size_t nty = nullity(A.impl(),U,S,V,thresh);
    if ( nty == 0 )
      return Matrix<value_type>(0,0);
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