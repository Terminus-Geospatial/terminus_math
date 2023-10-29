/**
 * @file    Matrix_Operations.hpp
 * @author  Marvin Smith
 * @date    9/16/2023
 */
#pragma once

// Terminus Libraries
#include "../types/Functors.hpp"
#include "../types/Math_Functors.hpp"
#include "../vector/Vector_Transpose.hpp"
#include "Matrix_Col.hpp"
#include "Matrix_Functors.hpp"
#include "Matrix_Vector_Product.hpp"

// C++ Libraries
#include <type_traits>

namespace tmns::math {

/**
 * Extract a column of a matrix as a vector.
 */
template <typename MatrixT>
Matrix_Col<MatrixT> select_col( Matrix_Base<MatrixT>& matrix,
                                size_t                col )
{
    return Matrix_Col<MatrixT>( matrix.impl(), col );
}

/**
 * Extract a column of a matrix as a vector (const overload).
 */
template <typename MatrixT>
Matrix_Col<const MatrixT> select_col( const Matrix_Base<MatrixT>& matrix,
                                      size_t                      col )
{
    return Matrix_Col<const MatrixT>( matrix.impl(), col );
}

/**
 * Extract a row of a matrix as a vector.
 */
template <typename MatrixT>
Matrix_Row<MatrixT> select_row( Matrix_Base<MatrixT>& matrix,
                                size_t                row )
{
    return Matrix_Row<MatrixT>( matrix.impl(), row );
}

/**
 * Extract a row of a matrix as a vector (const overload).
 */
template <typename MatrixT>
Matrix_Row<const MatrixT> select_row( const Matrix_Base<MatrixT>& matrix,
                                      size_t                      row )
{
    return Matrix_Row<const MatrixT>( matrix.impl(), row );
}

/**
 * Negate a matrix.
 */
template <typename MatrixT>
Matrix_Unary_Functor<MatrixT, Arg_Negation_Functor>
    operator - ( const Matrix_Base<MatrixT>& m )
{
    return Matrix_Unary_Functor<MatrixT, Arg_Negation_Functor>( m.impl() );
}

/**
 * Absolute of a matrix
 */
template <typename MatrixT>
Matrix_Unary_Functor<MatrixT, Arg_Abs_Functor>
   abs( const Matrix_Base<MatrixT>& m )
{
    return Matrix_Unary_Functor<MatrixT, Arg_Abs_Functor>( m.impl() );
}

/**
 * Elementwise sum of two matrices.
 */
template <typename Matrix1T,
          typename Matrix2T>
Matrix_Binary_Functor<Matrix1T, Matrix2T, Arg_Arg_Sum_Functor>
    elem_sum( const Matrix_Base<Matrix1T>& m1,
              const Matrix_Base<Matrix2T>& m2 )
{
    return Matrix_Binary_Functor<Matrix1T, Matrix2T, Arg_Arg_Sum_Functor>( m1.impl(), m2.impl() );
}

/**
 * Sum of a matrix and a matrix (same as elem_sum).
 */
template <typename Matrix1T,
          typename Matrix2T>
Matrix_Binary_Functor<Matrix1T, Matrix2T, Arg_Arg_Sum_Functor>
    operator + ( const Matrix_Base<Matrix1T>& m1,
                 const Matrix_Base<Matrix2T>& m2 )
{
    return elem_sum( m1, m2 );
}

/**
 * Elementwise sum of a scalar and a matrix.
 */
template <typename ScalarT,
          typename MatrixT>
Matrix_Unary_Functor<MatrixT,Val_Arg_Sum_Functor<ScalarT>>
    elem_sum( ScalarT                     s, 
              const Matrix_Base<MatrixT>& m ) requires Is_Scalar<ScalarT>::type
{
    return Matrix_Unary_Functor<MatrixT, Val_Arg_Sum_Functor<ScalarT> >( m.impl(), s );
}

/**
 * Elementwise sum of a matrix and a scalar.
 */
template <typename MatrixT,
          typename ScalarT>
Matrix_Unary_Functor<MatrixT, Arg_Val_Sum_Functor<ScalarT>>
    elem_sum( const Matrix_Base<MatrixT>& m,
              ScalarT                     s ) requires Is_Scalar<ScalarT>::type
{
    return Matrix_Unary_Functor<MatrixT, Arg_Val_Sum_Functor<ScalarT> >( m.impl(), s );
}

/**
 * Elementwise difference of two matrices.
 */
template <typename Matrix1T,
          typename Matrix2T>
Matrix_Binary_Functor<Matrix1T, Matrix2T, Arg_Arg_Difference_Functor>
    elem_diff( const Matrix_Base<Matrix1T>& m1,
               const Matrix_Base<Matrix2T>& m2 )
{
    return Matrix_Binary_Functor<Matrix1T, Matrix2T, Arg_Arg_Difference_Functor>( m1.impl(), m2.impl() );
}

/**
 * Difference of two matrices (same as elem_diff).
 */
template <typename Matrix1T,
          typename Matrix2T>
Matrix_Binary_Functor<Matrix1T, Matrix2T, Arg_Arg_Difference_Functor>
    operator - ( const Matrix_Base<Matrix1T>& m1,
                 const Matrix_Base<Matrix2T>& m2 )
{
    return elem_diff( m1, m2 );
}

/**
 * Elementwise difference of a scalar and a matrix.
 */
template <typename ScalarT,
          typename MatrixT>
Matrix_Unary_Functor<MatrixT, Val_Arg_Difference_Functor<ScalarT>>
    elem_diff( ScalarT                     s,
               const Matrix_Base<MatrixT>& m ) requires Is_Scalar<ScalarT>::type
{
    return Matrix_Unary_Functor<MatrixT, Val_Arg_Difference_Functor<ScalarT> >( m.impl(), s );
}

/**
 * Elementwise difference of a matrix and a scalar.
 */
template <typename ScalarT,
          typename MatrixT>
Matrix_Unary_Functor<MatrixT, Arg_Val_Difference_Functor<ScalarT>>
    elem_diff( const Matrix_Base<MatrixT>& m,
               ScalarT                     s ) requires Is_Scalar<ScalarT>::type
{
    return Matrix_Unary_Functor<MatrixT, Arg_Val_Difference_Functor<ScalarT> >( m.impl(), s );
}

/**
 * Elementwise product of two matrices.
 */
template <typename Matrix1T,
          typename Matrix2T>
Matrix_Binary_Functor<Matrix1T, Matrix2T, Arg_Arg_Product_Functor>
   elem_prod( const Matrix_Base<Matrix1T>& m1,
              const Matrix_Base<Matrix2T>& m2 )
{
    return Matrix_Binary_Functor<Matrix1T, Matrix2T, Arg_Arg_Product_Functor>( m1.impl(), m2.impl() );
}

/**
 * Elementwise product of a scalar and a matrix.
 */
template <typename ScalarT,
          typename MatrixT>
Matrix_Unary_Functor<MatrixT,Val_Arg_Product_Functor<ScalarT>>
    elem_prod( ScalarT                     s,
               const Matrix_Base<MatrixT>& m ) requires Is_Scalar<ScalarT>::value
{
    return Matrix_Unary_Functor<MatrixT, Val_Arg_Product_Functor<ScalarT> >( m.impl(), s );
}

/**
 * Product of a scalar and a matrix (same as elem_prod)
 */
template <typename ScalarT,
          typename MatrixT>
std::enable_if_t<Is_Scalar<ScalarT>::value,
                 Matrix_Unary_Functor<MatrixT, Val_Arg_Product_Functor<ScalarT>>>
    operator * ( ScalarT                    s,
                 const Matrix_Base<MatrixT>& m )
{
    return elem_prod( s, m );
}

/**
 * Elementwise product of a matrix and a scalar.
 */
template <typename ScalarT,
          typename MatrixT>
std::enable_if_t<Is_Scalar<ScalarT>::type,
                 Matrix_Unary_Functor<MatrixT, Arg_Val_Product_Functor<ScalarT>>>
    elem_prod( const Matrix_Base<MatrixT>& m,
               ScalarT                     s )
{
    return Matrix_Unary_Functor<MatrixT, Arg_Val_Product_Functor<ScalarT> >( m.impl(), s );
}

/**
 * Product of a matrix and a scalar (same as elem_prod).
 */
template <typename ScalarT,
          typename MatrixT>
std::enable_if_t<Is_Scalar<ScalarT>::type,
                 Matrix_Unary_Functor<MatrixT, Arg_Val_Product_Functor<ScalarT>>>
    operator * ( const Matrix_Base<MatrixT>& m,
                 ScalarT                     s )
{
    return elem_prod( m, s );
}


/**
 * Elementwise quotient of two matrices.
 */
template <typename Matrix1T,
          typename Matrix2T>
Matrix_Binary_Functor<Matrix1T, Matrix2T, Arg_Arg_Quotient_Functor>
   elem_quot( const Matrix_Base<Matrix1T>& m1,
              const Matrix_Base<Matrix2T>& m2 )
{
    return Matrix_Binary_Functor<Matrix1T,Matrix2T,Arg_Arg_Quotient_Functor>( m1.impl(), m2.impl() );
}

/**
 * Elementwise quotient of a scalar and a matrix
 */
template <typename ScalarT,
          typename MatrixT>
Matrix_Unary_Functor<MatrixT, Val_Arg_Quotient_Functor<ScalarT>>
   elem_quot( ScalarT                     s,
              const Matrix_Base<MatrixT>& m ) requires Is_Scalar<ScalarT>::type
{
    return Matrix_Unary_Functor<MatrixT, Val_Arg_Quotient_Functor<ScalarT> >( m.impl(), s );
}

/**
 * Elementwise quotient of a matrix and a scalar.
 */
template <typename ScalarT,
          typename MatrixT>
Matrix_Unary_Functor<MatrixT, Arg_Val_Quotient_Functor<ScalarT>>
    elem_quot( const Matrix_Base<MatrixT>& m,
               ScalarT                     s ) requires Is_Scalar<ScalarT>::type
{
    return Matrix_Unary_Functor<MatrixT, Arg_Val_Quotient_Functor<ScalarT>>( m.impl(), s );
}

/**
 * Quotient of a matrix and a scalar (same as elem_quot).
 */
template <typename ScalarT,
          typename MatrixT>
Matrix_Unary_Functor<MatrixT, Arg_Val_Quotient_Functor<ScalarT>>
    operator / ( const Matrix_Base<MatrixT>& m, ScalarT s ) requires Is_Scalar<ScalarT>::type
{
    return elem_quot( m, s );
}

/**
 * Product of a matrix and a vector
 */
template <typename MatrixT,
          typename VectorT>
Matrix_Vector_Product<MatrixT,VectorT,false>
    operator * ( const Matrix_Base<MatrixT>& m,
                 const Vector_Base<VectorT>& v )
{
    return Matrix_Vector_Product<MatrixT,VectorT,false>( m.impl(), v.impl() );
}

/**
 * Product of a transposed matrix and a vector
 */
template <typename MatrixT,
          typename VectorT>
Matrix_Vector_Product<MatrixT,VectorT,true>
    operator * ( const Matrix_Transpose<MatrixT>& m,
                 const Vector_Base<VectorT>&      v )
{
    return Matrix_Vector_Product<MatrixT,VectorT,true>( m.child(), v.impl() );
}

/**
 * Product of a transposed vector and a matrix
 */
template <typename VectorT,
          typename MatrixT>
Vector_Transpose<const Matrix_Vector_Product<MatrixT,VectorT,true> >
    operator * ( const Vector_Transpose<VectorT>& v,
                 const Matrix_Base<MatrixT>&      m )
{
    return transpose( Matrix_Vector_Product<MatrixT,VectorT,true>( m.impl(), v.child() ));
}

/**
 * Outer product of two vectors.
 */
template <class Vector1T, class Vector2T>
Matrix<typename Product_Type<typename Vector1T::value_type, 
                             typename Vector2T::value_type>::type,
                             (Vector_Size<Vector2T>::value?(Vector_Size<Vector1T>::value):0),
                             (Vector_Size<Vector1T>::value?(Vector_Size<Vector2T>::value):0)>
    outer_prod( const Vector_Base<Vector1T>& v1,
                const Vector_Base<Vector2T>& v2 )
{
    typedef Matrix<typename Product_Type<typename Vector1T::value_type,
                                         typename Vector2T::value_type>::type,
                                         (Vector_Size<Vector2T>::value?(Vector_Size<Vector1T>::value):0),
                                         (Vector_Size<Vector1T>::value?(Vector_Size<Vector2T>::value):0)> result_type;
    result_type result;
    result.set_size( v1.impl().size(), v2.impl().size() );
    for( size_t i=0; i<v1.impl().size(); ++i )
    for( size_t j=0; j<v2.impl().size(); ++j )
        result(i,j) = v1.impl()(i) * v2.impl()(j);
    return result;
}

/**
 * Matrix Inverse (2x2 case)
 */
template <typename T>
Matrix<T, 2, 2> inverse( const Matrix<T,2,2>& m)
{
    T const* d = m.data();
    T det = d[0]*d[3] - d[1]*d[2];
    if( det == T(0) )
    {
        throw std::runtime_error( "Matrix is singular in inverse()" );
    }
    Matrix<T, 2, 2> out( { d[3]/det, 
                           -d[1]/det,
                           -d[2]/det,
                           d[0]/det } );
    return out;
}
  
/**
 * Matrix inversion
 */
template <class MatrixT>
Matrix<typename MatrixT::value_type> inverse( const Matrix_Base<MatrixT>& m )
{
    using value_type = MatrixT::value_type;
    value_type zero = value_type();

    size_t size = m.impl().cols();
    MatrixN<value_type> buf = m;

    // Initialize the permutation
    VectorN<size_t> pm( size );
    for( size_t i=0; i<size; ++i )
    {
        pm(i) = i;
    }

    // Perform LU decomposition with partial pivoting
    for( size_t i = 0; i < size; ++i )
    {
        Matrix_Col<MatrixN<value_type> > mci( buf, i );
        Matrix_Row<MatrixN<value_type> > mri( buf, i );

        size_t i_norm_inf = i + index_norm_inf( subvector( mci, i, size - i ) );
        
        if( buf( i_norm_inf, i ) == zero )
        {
            throw std::runtime_error( "Matrix is singular in inverse()" );
        }
        if( i_norm_inf != i )
        {
            size_t pbuf = pm(i);
            pm(i) = pm(i_norm_inf);
            pm(i_norm_inf) = pbuf;
            VectorN<value_type> rowbuf = mri;
            mri = select_row( buf, i_norm_inf );
            select_row( buf, i_norm_inf ) = rowbuf;
        }
        if( i != size-1 )
        {
            subvector( mci, i+1, size-i-1 ) /= buf( i, i );
            submatrix( buf, i+1, i+1, size-i-1, size - i - 1 ) -= outer_prod( subvector( mci, i+1, size - i - 1 ),
                                                                              subvector( mri, i+1, size - i - 1 ) );
        }
    }

    // Build up a permuted identity matrix
    Matrix<value_type> inverse(size,size);
    for ( size_t i=0; i<size; ++i )
    {
        inverse( i, pm(i) ) = value_type(1);
    }

    // Divide by the lower-triangular term
    for( size_t i=0; i<size; ++i )
    {
        for( size_t j=0; j<size; ++j )
        {
            value_type t = inverse(i,j);
            if( t != zero )
            {
                for( size_t k = i + 1; k < size; ++k )
                {
                    inverse(k,j) -= buf(k,i) * t;
                }
            }
        }
    }

    // Divide by the upper-triangular term
    for( ssize_t i=size-1; i>=0; --i )
    {
        for( ssize_t j=size-1; j>=0; --j )
        {
            value_type t = inverse(i,j) /= buf(i,i);
            if( t != zero )
            {
                for( ssize_t k=i-1; k>=0; --k )
                {
                    inverse(k,j) -= buf(k,i) * t;
                }
            }
        }
    }

    return inverse;
  }

} // End of tmns::math namespace