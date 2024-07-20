/**
 * @file    SVD.hpp
 * @author  Marvin Smith
 * @date    11/13/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>
#include <terminus/math/matrix.hpp>

// Boost Libraries
#include <boost/numeric/conversion/cast.hpp>

namespace tmns::math::linalg {

namespace detail {
template <typename MatrixT,
          typename VectorT>
typename VectorT::value_type calc_threshold( const Matrix_Base<MatrixT>& A,
                                             const Vector_Base<VectorT>& S )
{
      return typename VectorT::value_type( 0.5
                * std::sqrt( double( A.impl().cols() ) + double( A.impl().rows() ) + 1. )
                * S.impl()[0]
                * std::numeric_limits<typename VectorT::value_type>::epsilon() );
}
} // End of detail namespace

/**
 * Compute the singular values of a Matrix A
 * 
 * S will be resized if necessary to the appropriate output dimensions
 * based on the dimensions of A.
 */
Result<VectorN<double>> svd( const MatrixN<double>& A );

/**
 * Compute the singular values of a Matrix A
 * 
 * S will be resized if necessary to the appropriate output dimensions
 * based on the dimensions of A.
 */
Result<VectorN<float>> svd( const MatrixN<float>& A );

Result<void> svd_impl( const MatrixN<double>& A,
                       MatrixN<double>&       U,
                       VectorN<double>&       S,
                       MatrixN<double>&       V );

/**
 * Decompose matrix using Singular Value Decomposition
 */
Result<void> complete_svd_impl( const MatrixN<double>& A,
                                MatrixN<double>&       U,
                                VectorN<double>&       S,
                                MatrixN<double>&       V );

/**
 * Decompose matrix using Singular Value Decomposition
 */
Result<void> complete_svd_impl( const MatrixN<float>& A,
                                MatrixN<float>&       U,
                                VectorN<float>&       S,
                                MatrixN<float>&       V );


/**
 * Compute the singular value decomposition of the matrix A.
 * 
 * U, S, and VT will be resized if necessary to the appropriate output dimensions
 * based on the dimensions of A.
 */
template <typename AMatrixT,
          typename UMatrixT,
          typename SingularValuesT,
          typename VTMatrixT>
Result<void> svd( const AMatrixT&  A,
                  UMatrixT&        U,
                  SingularValuesT& S,
                  VTMatrixT&       VT )
{
    return svd_impl( A, U, S, VT );    
}

/**
 * Compute the singular value decomposition of the matrix A, including
 * complete orthogonal bases of the domain and range even when A is rectangular.
 * 
 * U, S, and VT will be resized if necessary to the appropriate output dimensions
 * based on the dimensions of A.
 */
template <typename AMatrixT,
          typename UMatrixT,
          typename SingularValuesT,
          typename VTMatrixT>
Result<void> complete_svd( const AMatrixT&  A,
                           UMatrixT&        U,
                           SingularValuesT& S,
                           VTMatrixT&       VT )
{
    return complete_svd_impl( A, U, S, VT );
}

/**
 * Solve for the rank of a matrix .. using previous SVD results
 */
template <typename MatrixT,
          typename MatrixT2,
          typename VectorT>
int rank( const Matrix_Base<MatrixT>&   A,
          const Matrix_Base<MatrixT2>& /*U*/,
          const Vector_Base<VectorT>&   S,
          const Matrix_Base<MatrixT2>& /*V*/,
          typename VectorT::value_type thresh = -1 )
{
    using value_type = typename VectorT::value_type;
    
    value_type th = ( thresh >= 0. ? thresh : detail::calc_threshold(A, S) );
    int nr = 0;
    for( size_t j = 0; j < S.impl().size(); j++ )
    {
        if( S.impl()[j] > th )
        {
            nr++;
        }
    }
    return nr;
}

/**
 * Solve for the rank of a matrix.
 */
template <class MatrixT>
int rank( const Matrix_Base<MatrixT>& A,
          typename MatrixT::value_type thresh = -1 )
{
    using value_type = typename MatrixT::value_type;
    MatrixN<value_type> U, V;
    VectorN<value_type> S;
    complete_svd( A.impl(), U, S, V );
    V = transpose(V);
    return rank( A, U, S, V, thresh );
}

}  // End of tmns::math::linalg