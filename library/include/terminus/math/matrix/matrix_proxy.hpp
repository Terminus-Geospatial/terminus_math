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
 * @file    matrix_proxy.hpp
 * @author  Marvin Smith
 * @date    10/9/2023
 */
#pragma once

// Terminus Libraries
#include <terminus/math/matrix/matrix_base.hpp>

namespace tmns::math {

/**
 * @class Matrix_Proxy<ElementT,RowsN,ColsN>
 *
 * A fixed-dimension matrix proxy class, treating an arbitrary block of memory as a
 * Matrix in packed row-major format.
 *
 */
template <class ElementT,
          size_t RowsN = 0,
          size_t ColsN = 0>
class Matrix_Proxy : public Matrix_Base<Matrix_Proxy<ElementT,RowsN,ColsN> >
{
    public:

        /// Value Type
        using value_type = ElementT;

        /// Reference Type
        using reference_type = ElementT&;

        /// Const Reference Type
        using const_reference_type = const ElementT&;

        /// Iterator Type
        using iter_t = ElementT*;

        /// Const Iterator Type
        using const_iter_t = const ElementT*;

        /**
         * Constructor given a random pointer
         */
        Matrix_Proxy( ElementT* ptr )
            : m_ptr(ptr)
        {}

        /**
         * Copy Assignment Operator
         */
        Matrix_Proxy& operator = ( const Matrix_Proxy& m )
        {
            if( m.rows() != RowsN ||
                m.cols() != ColsN )
            {
                std::stringstream sout;
                sout << "Matrices are not the same size. This (" << RowsN << " x "
                     << ColsN << ") vs (" << m.rows() << " x " << m.cols() << ")";
                throw std::runtime_error( sout.str() );
            }
            Matrix<ElementT,RowsN,ColsN> tmp( m );
            std::copy( tmp.begin(), tmp.end(), begin() );
            return *this;
        }

        /**
         * Generalized Matrix Assignment Operator
         */
        template <typename T>
        Matrix_Proxy& operator = ( const Matrix_Base<T>& m )
        {
            if( m.impl().rows() != RowsN ||
                m.impl().cols() != ColsN )
            {
                std::stringstream sout;
                sout << "Matrix must have dims " << RowsN << " x " << ColsN << ". Actual: "
                     << m.impl().rows() << " x " << m.impl().cols();
                throw std::runtime_error( sout.str() );
            }
            Matrix<ElementT,RowsN,ColsN> tmp( m );
            std::copy( tmp.begin(), tmp.end(), begin() );
            return *this;
        }

        /**
         * Temporary-free generalized assignment operator, from arbitrary matrix
         * expressions. This is a performance-optimizing function to be used with caution!
         *
        template <typename T>
        Matrix_Proxy& operator = ( const Matrix_No_Tmp<T>& m )
        {
            if( m.impl().rows() != Rows ||
                m.impl().cols() != ColsN )
            {
                std::stringstream sout;
                sout << "Matrix must have dims " << RowsN << " x " << ColsN << ". Actual: "
                     << m.impl().rows() << " x " << m.impl().cols();
                throw std::runtime_error( sout.str() );
            }
            std::copy( m.impl().begin(), m.impl().end(), begin() );
            return *this;
        }*/

        /**
         * Get the number of matrix rows
         */
        size_t rows() const
        {
            return RowsN;
        }

        /**
         * Get the number of matrix columns
         */
        size_t cols() const
        {
            return ColsN;
        }

        /**
         * Change the size of the matrix.
         * Elements in memory are preserved when specified.
         */
        void set_size( size_t new_rows,
                       size_t new_cols,
                       bool /*preserve*/ = false )
        {
            if( new_rows != rows() || new_cols != cols() )
            {
                throw std::runtime_error( "Cannot resize Matrix_Proxy class." );
            }
        }

        /**
         * Function Operator
         */
        value_type& operator()( size_t row,
                                size_t col )
        {
            return m_ptr[ row * cols() + col ];
        }

        /**
         * Function Operator
         */
        value_type const& operator()( size_t row,
                                      size_t col ) const
        {
            return m_ptr[ row * cols() + col ];
        }

        /**
         * Get the beginning data pointer
         */
        value_type* data()
        {
            return &( operator()( 0, 0 ) );
        }

        /**
         * Get the beginning data pointer
         */
        const value_type* data() const
        {
            return &( operator()( 0, 0 ) );
        }

        /**
         * Get the beginning of the matrix
         */
        iter_t begin()
        {
            return m_ptr;
        }

        /**
         * Get the beginning of the matrix
         */
        const_iter_t begin() const
        {
            return m_ptr;
        }

        /**
         * Get the beginning of the matrix
         */
        iter_t end()
        {
            return m_ptr + rows() * cols();
        }

        /**
         * Get the end of the matrix
        */
        const_iter_t end() const
        {
            return m_ptr + rows() * cols();
        }

        /**
         * Get name
         */
        static std::string name()
        {
            std::stringstream sout;
            sout << "Matrix_Proxy<" << Data_Type_Name<ElementT>::name()
                 << "," << RowsN << "," << ColsN <<  ">";
            return sout.str();
        }

    private:

        ElementT* m_ptr { nullptr };

}; // End of Matrix_Proxy class

template <typename ElementT,
          size_t   RowsN,
          size_t   ColsN>
struct Matrix_Rows<Matrix_Proxy<ElementT,RowsN,ColsN>>
{
    static const size_t value = RowsN;
};

template <typename ElementT, size_t RowsN, size_t ColsN>
struct Matrix_Cols<Matrix_Proxy<ElementT,RowsN,ColsN> >
{
    static const size_t value = ColsN;
};


/**
 * @class MatrixProxy<ElementT>
 *
 * A arbitrary-dimension matrix proxy class, treating an arbitrary
 * block of memory as a Matrix in packed row-major format.
 *
 * An arbitrary-dimension matrix proxy class.
 */
template <typename ElementT>
class Matrix_Proxy<ElementT,0,0> : public Matrix_Base<Matrix_Proxy<ElementT>>
{
    public:

        /// @brief Value Type
        using value_type = ElementT;

        /// @brief Reference Type
        using reference_type = ElementT&;

        /// @brief Const Reference Type
        using const_reference_type = const ElementT&;

        /// @brief Iterator Type
        using iter_t = ElementT*;

        /// @brief Const Iterator Type
        using const_iter_t = const ElementT*;

        /**
         * Constructs a matrix with zero size.
         */
        Matrix_Proxy( ElementT* ptr,
                      size_t    rows,
                      size_t    cols )
            : m_ptr(ptr),
              m_rows(rows),
              m_cols(cols)
        {}

        /**
         * Construct a matrix with another container
         */
        template <typename ValueT>
        Matrix_Proxy( const Matrix_Base<ValueT>& matrix )
            : m_ptr(matrix.data()),
              m_rows(matrix.rows()),
              m_cols(matrix.cols())
        {}

        /**
         * Standard copy assignment operator.
         */
        Matrix_Proxy& operator = ( const Matrix_Proxy& m )
        {
            if( m.impl().rows() != rows() ||
                m.impl().cols() != cols() )
            {
                std::stringstream sout;
                sout << "Matrix must have dimensions " << rows() << "x" << cols() << ".";
                throw std::runtime_error( sout.str() );
            }
            Matrix<ElementT> tmp( m );
            std::copy( tmp.begin(), tmp.end(), begin() );
            return *this;
        }

        /**
         * Generalized assignment operator, from arbitrary matrix expressions.
         */
        template <typename T>
        Matrix_Proxy& operator=( const Matrix_Base<T>& m )
        {
            if( m.impl().rows() != rows() ||
                m.impl().cols() != cols() )
            {
                std::stringstream sout;
                sout << "Matrix must have dimensions " << rows() << "x" << cols() << ".";
                throw std::runtime_error( sout.str() );
            }
            Matrix<ElementT> tmp( m );
            std::copy( tmp.begin(), tmp.end(), begin() );
            return *this;
        }

        /**
         * Temporary-free generalized assignment operator, from arbitrary matrix expressions.
         * This is a performance-optimizing function to be used with caution!
         *
        template <typename T>
        Matrix_Proxy& operator=( const Matrix_No_Tmp<T>& m )
        {
            if( m.impl().rows() != rows() || m.impl().cols() != cols() )
            {
                std::stringstream sout;
                sout << "Matrix must have dimensions " << rows() << "x" << cols() << ".";
                throw std::runtime_error( sout.str() );
            }
            std::copy( m.impl().begin(), m.impl().end(), begin() );
            return *this;
        }*/

        /**
         * Get the matrix rows.
         */
        size_t rows() const
        {
            return m_rows;
        }

        /**
         * Get the matrix columns
         */
        size_t cols() const
        {
            return m_cols;
        }

        /**
         * Change the size of the matrix.
         * Elements in memory are preserved when specified.
         */
        void set_size( size_t new_rows,
                       size_t new_cols,
                       [[maybe_unused]] bool preserve = false )
        {
            if( new_rows != rows() || new_cols != cols() )
            {
                throw std::runtime_error( "Cannot resize Matrix_Proxy class." );
            }
        }

        /**
         * Access an element
         */
        value_type& operator()( size_t row,
                                size_t col )
        {
            return m_ptr[row*m_cols+col];
        }

        /**
         * Access an element
         */
        value_type const& operator()( size_t row,
                                      size_t col ) const
        {
            return m_ptr[row*m_cols+col];
        }

        /**
         * Get the data pointer
         */
        value_type *data()
        {
            return &( operator()( 0, 0 ) );
        }

        /**
         * Get the data pointer
         */
        const value_type* data() const
        {
            return &( operator()(0,0) );
        }

        /**
         * Get the beginning iterator position
         */
        iter_t begin()
        {
            return m_ptr;
        }

        /**
         * Get the beginning iterator position
         */
        const_iter_t begin() const
        {
            return m_ptr;
        }

        /**
         * Get the end iterator position
         */
        iter_t end()
        {
            return m_ptr + rows() * cols();
        }

        /**
         * Get the end iterator position
         */
        const_iter_t end() const
        {
            return m_ptr + rows() * cols();
        }

        /**
         * Get name
         */
        static std::string name()
        {
            std::stringstream sout;
            sout << "Matrix_Proxy<" << Data_Type_Name<ElementT>::name()
                 << ",0,0>";
            return sout.str();
        }

    private:

        /// Data Pointer
        ElementT* m_ptr { nullptr };

        /// Matrix Rows
        size_t m_rows;

        /// Matrix Columns
        size_t m_cols;

}; // End of Matrix_Proxy Class

/**
 * Shallow view of an image as a matrix.  Returns a Matrix_Proxy object with the same
 * element type as the channel type in the original image.
 */
template <typename ContainerT>
Matrix_Proxy<typename ContainerT::value_type>  matrix_proxy( const ContainerT& container )
{
    return Matrix_Proxy<typename ContainerT::value_type>( container );
}

/**
 * Shallow view of an image as a matrix.  Returns a Matrix_Proxy object with the same
 * element type as the channel type in the original image.
 */
template <typename DataT>
Matrix_Proxy<DataT> matrix_proxy( DataT* data_ptr,
                                  size_t rows,
                                  size_t cols )
{
    return Matrix_Proxy<DataT>( data_ptr, rows, cols );
}

} // End of tmns::math namespace