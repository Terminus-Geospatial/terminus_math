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
 * @file    Matrix_Transpose.hpp
 * @author  Marvin Smith
 * @date    10/10/2023
 */
#pragma once

// Terminus Math Libraries
#include "indexing_matrix_iterator.hpp"

namespace tmns::math {

// Forward declar matrix
template <typename ElementT,
          size_t   RowsN,
          size_t   ColsN>
class Matrix;

/**
 * @class Matrix_Transpose<MatrixT>
 * A transposed matrix class.
 */
template <typename MatrixT>
class Matrix_Transpose : public Matrix_Base<Matrix_Transpose<MatrixT> >
{
    public:

        /// @brief Value Type
        using value_type = MatrixT::value_type;

        /// @brief Reference Type
        using reference_type = MatrixT::reference_type;

        /// @brief Const Reference Type
        using const_reference_type = MatrixT::const_reference_type;

        /// @brief Iterator Type
        using iter_t = Indexing_Matrix_Iterator<Matrix_Transpose<MatrixT>>;

        /// @brief Const Iterator Type
        using const_iter_t = Indexing_Matrix_Iterator<const Matrix_Transpose<MatrixT>>;

        /**
         * Constructs a matrix transpose.
         */
        Matrix_Transpose( MatrixT& matrix )
            : m_matrix( matrix ) {}

        /**
         * Standard copy assignment operator.
         */
        Matrix_Transpose& operator = ( const Matrix_Transpose& m )
        {
            if( m.impl().rows() != rows() || m.impl().cols() != cols() )
            {
                std::stringstream sout;
                sout << "Matrix must have dimensions " << rows() << "x" << cols() << ".";
                throw std::runtime_error( sout.str() );
            }
            Matrix<value_type,0,0> tmp( m );
            std::copy( tmp.begin(), tmp.end(), begin() );
            return *this;
        }

        /**
         * Generalized assignment operator, from arbitrary matrix expressions.
         */
        template <class T>
        Matrix_Transpose& operator = ( const Matrix_Base<T>& m )
        {
            if( m.impl().rows() != rows() || m.impl().cols() != cols() )
            {
                std::stringstream sout;
                sout << "Matrix must have dimensions " << rows() << "x" << cols() << ".";
                throw std::runtime_error( sout.str() );
            }
            Matrix<value_type,0,0> tmp( m );
            std::copy( tmp.begin(), tmp.end(), begin() );
            return *this;
        }

        /**
         * Temporary-free generalized assignment operator, from arbitrary VW matrix expressions.
         *  This is a performance-optimizing function to be used with caution!
         *
        template <typename T>
        Matrix_Transpose& operator=( const Matrix_No_Tmp<T>& m )
        {
            if( m.impl().rows() != rows() || m.impl().cols() != cols() )
            {
                std::stringstream sout;
                sout << "Matrix must have dimensions " << rows() << "x" << cols() << ".";
                throw std::runtime_error( sout.str() );
            }
            std::copy( m.impl().begin(),
                       m.impl().end(),
                       begin() );
            return *this;
        }*/

        /**
         * Returns the underlying non-transposed matrix.
         */
        MatrixT& child()
        {
            return m_matrix;
        }

        /**
         * Returns the underlying non-transposed matrix.
         */
        MatrixT const& child() const
        {
            return m_matrix;
        }

        /**
         * Get the matrix rows
        */
        size_t rows() const
        {
            return m_matrix.cols();
        }

        /**
         * Get the matrix columns
         */
        size_t cols() const
        {
            return m_matrix.rows();
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
                throw std::runtime_error( "Cannot resize Matrix_Transpose class." );
            }
        }

        /**
         * Function Operator
         */
        reference_type operator()( size_t row,
                                   size_t col )
        {
            return child()( col, row );
        }

        /**
         * Function Operator
         */
        const_reference_type operator()( size_t row,
                                         size_t col ) const
        {
            return child()( col, row );
        }

        /**
         * Get the start iterator position
         */
        iter_t begin()
        {
            return iter_t( *this, 0, 0 );
        }

        /**
         * Get the start iterator position
         */
        const_iter_t begin() const
        {
            return const_iter_t( *this, 0, 0 );
        }

        /**
         * Get the end iterator position
         */
        iter_t end()
        {
            return iter_t( *this, rows(), 0 );
        }

        /**
         * Get the end iterator position
         */
        const_iter_t end() const
        {
            return const_iter_t( *this, rows(), 0 );
        }

        /**
         * Get name
         */
        static std::string name()
        {
            std::stringstream sout;
            sout << "Matrix_Transpose<" << MatrixT::name() <<  ">";
            return sout.str();
        }

    private:

        /// @brief Underlying Matrix Data
        MatrixT &m_matrix;

}; // End of Matrix_Transpose Class


/**
 * Construct Matrix transpose.
 */
template <typename MatrixT>
Matrix_Transpose<MatrixT> transpose( Matrix_Base<MatrixT>& m )
{
    return Matrix_Transpose<MatrixT>( m.impl() );
}

/**
 * Construct Matrix transpose (const overload).
 */
template <typename MatrixT>
Matrix_Transpose<const MatrixT> transpose( const Matrix_Base<MatrixT>& m )
{
    return Matrix_Transpose<const MatrixT>( m.impl() );
}

/**
 * Matrix transpose (transpose overload).
 */
template <typename MatrixT>
MatrixT& transpose( Matrix_Transpose<MatrixT>& m )
{
    return m.child();
}

/**
 * Matrix transpose (const transpose overload).
 */
template <typename MatrixT>
MatrixT const& transpose( const Matrix_Transpose<MatrixT>& m )
{
    return m.child();
}

} // End of tmns::math namespace