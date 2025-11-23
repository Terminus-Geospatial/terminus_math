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
 * @file    Matrix.hpp
 * @author  Marvin Smith
 * @date    9/16/2023
 */
#pragma once

// Terminus Math Libraries
#include "../types/Fundamental_Types.hpp"
#include "Matrix_Base.hpp"
#include "Matrix_Col.hpp"
#include "Matrix_Row.hpp"
#include "Matrix_Transpose.hpp"

// Terminus Libraries
#include <terminus/log/utility.hpp>

// Boost Libraries
#include <boost/mpl/min_max.hpp>

// C++ Libraries
#include <cassert>
#include <iterator>
#include <ranges>
#include <stack>
#include <stdexcept>

namespace tmns::math {

/**
 * A statically-allocated fix-dimension matrix class.
 */
template <typename ElementT,
          size_t   RowsN = 0,
          size_t   ColsN = 0>
class Matrix : public Matrix_Base<Matrix<ElementT,RowsN,ColsN> >
{
    public:

        /// @brief Array Type
        using array_type = std::array<ElementT, RowsN * ColsN>;

        /// @brief Value Element Type
        using value_type = ElementT;

        /// @brief Reference Type
        using reference_type = ElementT&;

        /// @brief Const Reference Type
        using const_reference_type = const ElementT&;

        /// @brief Standard Iterator Type
        using iter_t = typename array_type::iterator;

        /// @brief Const Iterator Type
        using const_iter_t = typename array_type::const_iterator;

        /**
         * Default Constructor
         */
        Matrix()
        {
            std::fill( m_data.begin(),
                       m_data.end(),
                       0 );
        }

        /**
         * Constructor given an array of data
         */
        Matrix( std::array<value_type,RowsN*ColsN> data )
            : m_data( std::move( data ) )
        {
            // Fill in the remaining items
            auto pos = m_data.begin();
            std::advance( pos, RowsN * ColsN );

            std::fill( pos,
                       m_data.end(),
                       0 );
        }

        /**
         * Constructor given an array of data
         */
        template <typename OtherValueT,
                  size_t ArrayDims>
        Matrix( std::array<OtherValueT,ArrayDims> data ) requires ( ArrayDims <= ColsN * RowsN )
        {
            // Copy the first X elements
            std::copy( data.begin(),
                       data.end(),
                       m_data.begin() );

            // Fill in the remaining items
            auto pos = m_data.begin();
            std::advance( pos, ArrayDims );

            std::fill( pos,
                       m_data.end(),
                       0 );
        }

        /**
         * Constructor given an STL style container of data
         */
        template <typename ContainerT>
        Matrix( const ContainerT& data ) requires (Is_STL_Container<ContainerT>::value)
        {
            if( data.size() < RowsN * ColsN )
            {
                std::stringstream sout;
                sout << "Array has less data than required matrix size. Required: " << RowsN * ColsN
                     << ", Actual: " << data.size() << ", Remaining fields will be zero-filled";
                tmns::log::warn( sout.str() );
            }
            // Copy the first X elements
            std::copy( data.begin(),
                       data.end(),
                       m_data.begin() );

            // Fill in the remaining items
            auto pos = m_data.begin();
            std::advance( pos, RowsN * ColsN );

            std::fill( pos,
                       m_data.end(),
                       0 );
        }

        /**
         * Copy Constructor
         */
        Matrix( const Matrix& mat )
            : m_data( mat.m_data )
        {
        }

        /**
         * Copy Constructor from another Base Matrix-Type
         */
        template <typename OtherMatrixT>
        Matrix( const Matrix_Base<OtherMatrixT>& mat )
        {
            // Log error if incoming matrix is of incorrect size
            if( mat.impl().rows() != RowsN ||
                mat.impl().cols() != ColsN )
            {
                std::cerr << "Matrix must have dimensions " << RowsN << " rows by "
                          << ColsN << " cols.";
            }
            else
            {
                // Copy elements
                std::copy( mat.impl().begin(),
                           mat.impl().end(),
                           begin() );
            }
        }

        /**
         * Copy assignment operator
         */
        Matrix& operator = ( const Matrix& rhs )
        {
            Matrix temp( rhs );
            m_data = temp.m_data;
            return (*this);
        }

        /**
         * General assignment operator
         */
        template <typename OtherMatrixT>
        Matrix& operator = ( const Matrix_Base<OtherMatrixT>& mat )
        {
            if( mat.impl().rows() != RowsN ||
                mat.impl().cols() != ColsN )
            {
                std::cerr << "Matrix must have dimensions " << RowsN << " rows x " << ColsN
                          << " cols.  Actual: " << mat.impl().rows() << " rows x " << mat.impl().cols()
                          << " cols.  Skipping assignment.";
                return (*this);
            }

            // Create a new instance
            Matrix tmp( mat );
            m_data = tmp.m_data;
            return *this;
        }

        /**
         * Temporary-free generalized assignment operator, from arbitrary matrix
         * expressions. This is a performance-optimizing function to be used with
         * caution!
         */
        template <typename OtherMatrixT>
        Matrix& operator = ( const Matrix_No_Tmp<OtherMatrixT>& mat )
        {
            if( mat.impl().rows() != RowsN ||
                mat.impl().cols() != ColsN )
            {
                std::cerr << "Matrix must have dimensions " << RowsN << " rows x " << ColsN
                          << " cols.  Actual: " << mat.impl().rows() << " rows x " << mat.impl().cols()
                          << " cols.  Skipping assignment.";
                return (*this);
            }

            std::copy( mat.impl().begin(),
                       mat.impl().end(),
                       begin() );
            return *this;
        }

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
         * Resize the matrix
         */
        bool set_size( size_t new_rows,
                       size_t new_cols,
                       [[maybe_unused]] bool preserve = false )
        {
            if( new_rows != rows() ||
                new_cols != cols() )
            {
                std::cerr << "Cannot resize a fixed-size matrix.";
                return false;
            }
            return true;
        }

        /**
         * Access a specific element
         */
        value_type& operator()( size_t row,
                                size_t col )
        {
            if( row >= RowsN )
            {
                std::stringstream sout;
                sout << "Row: " << row << " > RowsN: " << RowsN;
                throw std::runtime_error( sout.str() );
            }
            if( col >= ColsN )
            {
                std::stringstream sout;
                sout << "Col: " << col << " > ColsN: " << ColsN;
                throw std::runtime_error( sout.str() );
            }
            return m_data[ row * ColsN + col ];
        }

        /**
         * Access a specific element
         */
        value_type const& operator()( size_t row,
                                      size_t col ) const
        {
            if( row >= RowsN )
            {
                std::stringstream sout;
                sout << "Row: " << row << " > RowsN: " << RowsN;
                throw std::runtime_error( sout.str() );
            }
            if( col >= ColsN )
            {
                std::stringstream sout;
                sout << "Col: " << col << " > ColsN: " << ColsN;
                throw std::runtime_error( sout.str() );
            }
            return m_data[ row * ColsN + col ];
        }

        /**
         * Get pointer to underlying data
         */
        value_type* data()
        {
            // This is designed to function when the underlying type isn't an array.
            return &(operator()(0,0));
        }

        /**
         * Get pointer to underlying data
         */
        const value_type* data() const
        {
            return &(operator()(0,0));
        }

        /**
         * Get the beginning of the matrix
         */
        iter_t begin()
        {
            return m_data.begin();
        }

        /**
         * Get the beginning of the matrix (const)
         */
        const_iter_t begin() const
        {
            return m_data.begin();
        }

        /**
         * Get the end of the matrix
         */
        iter_t end()
        {
            return m_data.end();
        }

        /**
         * Get the end of the matrix (const)
         */
        const_iter_t end() const
        {
            return m_data.end();
        }

        /**
         * Return the transpose
         */
        Matrix_Transpose<Matrix<ElementT,RowsN,ColsN>> T()
        {
            return transpose( *this );
        }

        /**
         * Return all diagonal components
         */
        Vector_<ElementT,std::min(RowsN,ColsN)> diagonal() const
        {
            Vector_<ElementT,std::min(RowsN,ColsN)> output;
            for( size_t x = 0; x < output.size(); x++ )
            {
                output[x] = this->operator()( x, x );
            }
            return std::move( output );
        }

        /**
         * Get the Determinant
         *
         * Optimized for 2x2 matrices
         */
        value_type determinant() const requires( RowsN == 2 && ColsN == 2 )
        {
            return m_data[0] * m_data[3] - m_data[1] * m_data[2];
        }

        /**
         * Get the Determinant
         * General case
         */
        value_type determinant() const
        {
            value_type result = value_type();

            // Make sure the matrix is square
            if( rows() != cols() )
            {
                tmns::log::error( ADD_CURRENT_LOC(),
                                  "Matrix must be square.  Actual: ",
                                  rows(), " x ", cols() );
                return result;
            }

            std::stack<std::pair<Matrix<value_type>,value_type> > s;

            s.push( std::make_pair( (*this), 1 ) );
            while( !s.empty() )
            {
                auto a = s.top().first;
                value_type scale = s.top().second;

                s.pop();

                // Make sure matrix is square
                if( a.rows() != a.cols() )
                {
                    tmns::log::error( ADD_CURRENT_LOC(),
                                  "Matrix must be square.  Actual: ",
                                  a.rows(), " x ", a.cols() );
                    return result;
                }

                size_t dim = a.rows();
                Matrix<value_type> sub;
                switch( dim )
                {
                    case 0:
                        break;
                    case 1:
                        result += scale * a( 0, 0 );
                        break;
                    case 2:
                        result += scale * ( a( 0, 0 ) * a( 1, 1 ) - a( 0, 1 ) * a( 1, 0 ) );
                        break;
                    default:
                        {
                            sub = submatrix( a, 1, 1, dim - 1, dim - 1 );
                            s.push( std::make_pair( sub, scale * a( 0, 0 ) ) );
                            scale *= -1;
                        }
                        for( size_t i = 1; i < ( dim - 1 ); ++i )
                        {
                            submatrix( sub, 0, 0, dim-1, i )       = submatrix( a, 1,   0, dim-1, i );
                            submatrix( sub, 0, i, dim-1, dim-i-1 ) = submatrix( a, 1, i+1, dim-1, dim-i-1 );
                            s.push( std::make_pair( sub, scale * a( 0, i ) ) );
                            scale *= -1;
                        }
                        {
                            sub = submatrix( a, 1, 0, dim-1, dim-1 );
                            s.push( std::make_pair( sub, scale * a( 0, dim - 1 ) ) );
                        }
                        break;
                } // End of switch statement
            }
            return result;
        }

        /**
         * Inverse Matrix
         */
        Matrix inverse() const
        {
            value_type zero = value_type();
            size_t sz     = cols();
            Matrix<value_type> buf = (*this);

            // Initialize the permutation
            VectorN<size_t> pm( sz );
            for( size_t i = 0; i < sz; ++i )
            {
                pm(i) = i;
            }

            // Perform LU decomposition with partial pivoting
            for( size_t i = 0; i < sz; ++i )
            {
                Matrix_Col<Matrix<value_type> > mci( buf, i );
                Matrix_Row<Matrix<value_type> > mri( buf, i );

                size_t i_norm_inf = i + index_norm_inf( subvector( mci, i, sz - i ) );

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
                    mri = select_row(buf,i_norm_inf);
                    select_row( buf, i_norm_inf ) = rowbuf;
                }
                if ( i != sz - 1 )
                {
                    subvector( mci, i+1, sz-i-1 ) /= buf( i, i );
                    submatrix( buf, i+1, i+1, sz-i-1, sz-i-1 ) -= outer_prod( subvector( mci, i+1, sz-i-1 ),
                                                                              subvector( mri, i+1, sz-i-1 ) );
                }
            }

            // Build up a permuted identity matrix
            Matrix<value_type> inverse_mat( sz, sz );
            for( size_t i = 0; i < sz; ++i )
            {
                inverse_mat( i, pm(i) ) = value_type(1);
            }

            // Divide by the lower-triangular term
            for( size_t i = 0; i < sz; ++i ){
            for( size_t j = 0; j < sz; ++j ){
                value_type t = inverse_mat( i, j );
                if( t != zero )
                {
                    for( size_t k = i+1; k < sz; ++k )
                    {
                        inverse_mat( k, j ) -= buf( k, i ) * t;
                    }
                }
            }} // End of lower-triangle division

            // Divide by the upper-triangular term
            for ( ssize_t i = static_cast<ssize_t>(sz - 1); i >= 0; --i ){
            for ( ssize_t j = static_cast<ssize_t>(sz - 1); j >= 0; --j ){
                value_type t = inverse_mat(i,j) /= buf(i,i);
                if( t != zero )
                {
                    for( ssize_t k = i-1; k >= 0; --k )
                    {
                        inverse_mat(k,j) -= buf(k,i) * t;
                    }
                }
            }}

            return inverse_mat;
        }

        /**
         * Return a zero matrix
         */
        static Matrix zeros()
        {
            return Matrix();
        }

        /**
         * Return a 1 matrix
         */
        static Matrix ones()
        {
            auto output = Matrix();
            for( auto it = output.begin(); it != output.end(); it++ )
            {
                (*it) = 1;
            }
            return output;
        }

        /**
         * Return an identity matrix
         */
        static Matrix identity()
        {
            // Create a zero matrix
            auto output = zeros();

            // Set 1 column
            size_t diag = std::min( output.cols(), output.rows() );
            for( size_t d = 0; d < diag; d++ )
            {
                output.impl()( d, d ) = ( value_type( 1 ) );
            }
            return output;
        }

        /**
         * Get name
         */
        static std::string name()
        {
            std::stringstream sout;
            sout << "Matrix<" << Data_Type_Name<ElementT>::name()
                 << "," << RowsN << "," << ColsN <<  ">";
            return sout.str();
        }

    private:

        /// Array Information
        array_type m_data;

}; // End of Matrix class

using Matrix_3x3 = Matrix<double,3,3>;
using Matrix_4x4 = Matrix<double,4,4>;

// Overload a few type traits

/**
 * Matrix implementation for Matrix_Rows
 */
template <typename ElementT,
          size_t RowsN,
          size_t ColsN>
struct Matrix_Rows<Matrix<ElementT,RowsN,ColsN> >
{
    static const size_t value = RowsN;
};

/**
 * Matrix implementation for Matrix_Rows
 */
template <typename ElementT,
          size_t RowsN,
          size_t ColsN>
struct Matrix_Cols<Matrix<ElementT,RowsN,ColsN> >
{
    static const size_t value = ColsN;
};

} // End of tmns::math namespace