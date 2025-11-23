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
 * @file    Matrix_Matrix_Product.hpp
 * @author  Marvin Smith
 * @date    10/11/2023
 */
#pragma once

// Terminus Math Libraries
#include <terminus/math/types/type_deduction.hpp>
#include <terminus/math/matrix/matrix_base.hpp>
#include <terminus/math/matrix/matrix_operations.hpp>
#include <terminus/math/matrix/matrix_proxy.hpp>

namespace tmns::math {

/**
 * Matrix matrix product.
 *
 * A class representing a product of a matrix and a vector.
 */
template <typename Matrix1T,
          typename Matrix2T,
          bool     Transpose1N,
          bool     Transpose2N>
class Matrix_Matrix_Product : public Matrix_Base<Matrix_Matrix_Product<Matrix1T,
                                                                       Matrix2T,
                                                                       Transpose1N,
                                                                       Transpose2N> >
{
    public:

        /// @brief Type of value
        using value_type = Product_Type<typename Matrix1T::value_type,
                                        typename Matrix2T::value_type>::type;

        /// @brief Reference Type
        using reference_type = value_type;

        /// @brief Const Reference Type
        using const_reference_type = value_type;

        /// @brief Iterator Type
        using iter_t = Indexing_Matrix_Iterator<const Matrix_Matrix_Product>;

        /// @brief Const iterator type
        using const_iter_t = iter_t;

        /**
         * Constructor
        */
        Matrix_Matrix_Product( const Matrix1T& m1,
                               const Matrix2T& m2 )
            : m_matrix1(m1),
              m_matrix2(m2)
        {}

        /**
         * Get the matrix rows
         */
        size_t rows() const
        {
            return (Transpose1N) ? ( m_matrix1.cols() ) : ( m_matrix1.rows() );
        }

        /**
         * Get the matrix columns
         */
        size_t cols() const
        {
            return (Transpose2N) ? ( m_matrix2.rows() ) : ( m_matrix2.cols() );
        }

        /**
         * Get the function operator (Non transposed matrices)
         */
        reference_type operator()( size_t i,
                                   size_t j ) const requires ( !Transpose1N && !Transpose2N )
        {
            return VectorN<value_type>::dot( select_row( m_matrix1, i ),
                                             select_col( m_matrix2, j ) );
        }

        /**
         * Get the function operator (1st matrix not transposed, 2nd matrix transposed)
         */
        reference_type operator()( size_t i,
                                   size_t j ) const requires ( !Transpose1N && Transpose2N )
        {
            return VectorN<value_type>::dot( select_row( m_matrix1, i ),
                                             select_row( m_matrix2, j ) );
        }

        /**
         * Get the function operator (1st matrix transposed, 2nd matrix not transposed)
         */
        reference_type operator()( size_t i,
                                   size_t j ) const requires ( Transpose1N && !Transpose2N )
        {
            return VectorN<value_type>::dot( select_col( m_matrix1, i ),
                                             select_col( m_matrix2, j ) );
        }

        /**
         * Get the function operator (Both matrices transposed)
         */
        reference_type operator()( size_t i,
                                   size_t j ) const requires ( Transpose1N && Transpose2N )
        {
            return VectorN<value_type>::dot( select_col( m_matrix1, i ),
                                             select_row( m_matrix2, j ) );
        }

        /**
         * Get the beginning iterator
         */
        iter_t begin() const
        {
            return iter_t( *this, 0, 0 );
        }

        /**
         * Get the end iterator
         */
        iter_t end() const
        {
            return iter_t( *this, rows(), 0 );
        }

        /**
         * Return all diagonal components
         */
        VectorN<value_type> diagonal() const
        {
            VectorN<value_type> output( std::min( rows(), cols() ) );
            for( size_t x = 0; x < output.size(); x++ )
            {
                output[x] = this->operator()( x, x );
            }
            return output;
        }

        /**
         * Get name
         */
        static std::string name()
        {
            std::stringstream sout;
            sout << "Matrix_Matrix_Product<" << Matrix1T::name() << "," << Matrix2T::name() << ","
                 << std::boolalpha << Transpose1N << "," << Transpose2N << ">";
            return sout.str();
        }

    private:

        template <typename MatrixT>
        struct Matrix_Closure
        {
            using type = Matrix<typename MatrixT::value_type>;
        };

        template <typename ElementT,
                  size_t   RowsN,
                  size_t ColsN>
        struct Matrix_Closure<Matrix<ElementT,RowsN,ColsN>>
        {
            using type = Matrix<ElementT,RowsN,ColsN> const&;
        };

        template <typename ElementT,
                  size_t   RowsN,
                  size_t   ColsN>
        struct Matrix_Closure<const Matrix<ElementT,RowsN,ColsN>>
        {
            using type = Matrix<ElementT,RowsN,ColsN> const&;
        };

        template <typename ElementT,
                  size_t   RowsN,
                  size_t   ColsN>
        struct Matrix_Closure<Matrix_Proxy<ElementT,RowsN,ColsN> >
        {
            using type = Matrix_Proxy<ElementT,RowsN,ColsN> const&;
        };

        template <typename ElementT,
                  size_t   RowsN,
                  size_t   ColsN>
        struct Matrix_Closure<const Matrix_Proxy<ElementT,RowsN,ColsN> >
        {
            using type = Matrix_Proxy<ElementT,RowsN,ColsN> const&;
        };

        // Internal matrices to multiply
        typename Matrix_Closure<Matrix1T>::type m_matrix1;
        typename Matrix_Closure<Matrix2T>::type m_matrix2;

}; // End of Matrix_Matrix_Product Class

/**
 * Compute the product of 2 matrices
 */
template <typename Matrix1T,
          typename Matrix2T>
Matrix_Matrix_Product<Matrix1T,Matrix2T,true,true>
    operator * ( const Matrix_Transpose<Matrix1T>& m1,
                 const Matrix_Transpose<Matrix2T>& m2 )
{
    return Matrix_Matrix_Product<Matrix1T,Matrix2T,true,true>( m1, m2 );
}

/**
 * Compute the product of 2 matrices
 */
template <typename Matrix1T,
          typename Matrix2T>
Matrix_Matrix_Product<Matrix1T,Matrix2T,false,true>
    operator * ( const Matrix_Base<Matrix1T>&      m1,
                 const Matrix_Transpose<Matrix2T>& m2 )
{
    return Matrix_Matrix_Product<Matrix1T,Matrix2T,false,true>( m1.impl(), m2 );
}

/**
 * Compute the product of 2 matrices
 */
template <typename Matrix1T,
          typename Matrix2T>
Matrix_Matrix_Product<Matrix1T,Matrix2T,true,false>
    operator * ( const Matrix_Transpose<Matrix1T>& m1,
                 const Matrix_Base<Matrix2T>&      m2 )
{
    return Matrix_Matrix_Product<Matrix1T,Matrix2T,true,false>( m1, m2.impl() );
}

/**
 * Compute the product of 2 matrices
 */
template <typename Matrix1T,
          typename Matrix2T>
Matrix_Matrix_Product<Matrix1T,Matrix2T,false,false>
    operator * ( const Matrix_Base<Matrix1T>& m1,
                 const Matrix_Base<Matrix2T>& m2 )
{
    return Matrix_Matrix_Product<Matrix1T,Matrix2T,false,false>( m1.impl(), m2.impl() );
}


} // End of tmns::math namespace