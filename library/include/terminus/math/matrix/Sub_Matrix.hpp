/**
 * @file    Sub_Matrix.hpp
 * @author  Marvin Smith
 * @date    9/18/2023
 */
#pragma once

// Terminus Libraries
#include "Indexing_Matrix_Iterator.hpp"
#include "Matrix_Base.hpp"

namespace tmns::math {

/**
 * View-like wrapper around another matrix to allow for sub-matrix operations
*/
template <typename MatrixT>
class Sub_Matrix : public Matrix_Base<Sub_Matrix<MatrixT> >
{
    public:

        // Underlying Datatype
        using value_type = typename MatrixT::value_type;

        /// Type to store for reference lookups
        using reference_type = std::conditional_t<std::is_const_v<MatrixT>,
                                                  typename MatrixT::const_reference_type,
                                                  typename MatrixT::reference_type>;
    
        /// @brief  Const reference type
        using const_reference_type = typename MatrixT::const_reference_type;

        /// @brief  Iterator Type
        using iter_t = Indexing_Matrix_Iterator<std::conditional_t<std::is_const_v<MatrixT>,
                                                                   const Sub_Matrix,
                                                                   Sub_Matrix>>;

        /// @brief Const Iterator Type
        using const_iter_t = Indexing_Matrix_Iterator<const Sub_Matrix>;

        /**
         * Constructor
         */
        Sub_Matrix( MatrixT& m,
                    size_t   row,
                    size_t   col,
                    size_t   rows,
                    size_t   cols )
            : m_matrix( m ),
              m_row( row ),
              m_col( col ),
              m_rows( rows ),
              m_cols( cols )
        {}

        /**
         * Standard Copy/Assignment Operator
         */
        Sub_Matrix& operator = ( const Sub_Matrix& m )
        {
            if( rows() != rows() ||
                cols() != cols() )
            {
                tmns::log::error( "Matrices must have the same dimensions in submatrix assignment. this: ",
                                  rows(), " rows x ", cols(), " cols. Actual: ", m.rows(), " rows x ",
                                  m.cols(), " cols.  Will be no-op." );
                return (*this);
            }
            Matrix<value_type> tmp( m );
            std::copy( tmp.begin(), tmp.end(), begin() );
            return (*this);
        }

        /**
         * Generalized assignment operator
         */
        template <typename OtherMatrixT>
        Sub_Matrix& operator = ( const Matrix_Base<OtherMatrixT>& m )
        {
            if( m.impl().rows() != rows() ||
                m.impl().cols() != cols() )
            {
                tmns::log::error( "Matrices must have the same dimensions in submatrix assignment. this: ",
                                  rows(), " rows x ", cols(), " cols. Actual: ", m.impl().rows(), " rows x ",
                                  m.impl().cols(), " cols.  Will be no-op." );
                return (*this);
            }
            Matrix<value_type> tmp( m );
            std::copy( tmp.begin(), tmp.end(), begin() );
            return *this;
        }

        /**
         * Temporary-free generalized assignment operator.
         * This is a performance-optimizing function to be used with caution!
         */
        template <typename OtherMatrixT>
        Sub_Matrix& operator = ( const Matrix_No_Tmp<OtherMatrixT>& m )
        {
            if( m.rows() != rows() ||
                m.cols() != cols() )
            {
                tmns::log::error( "Matrices must have the same dimensions in submatrix assignment. this: ",
                                  rows(), " rows x ", cols(), " cols. Actual: ", m.impl().rows(), " rows x ",
                                  m.impl().cols(), " cols.  Will be no-op." );
                return (*this);
            }
            std::copy( m.impl().begin(), m.impl().end(), begin() );
            return *this;
        }

        /**
         * Get internal matrix
         */
        MatrixT& child()
        {
            return m_matrix;
        }

        /**
         * Get internal matrix (const)
         */
        MatrixT const& child() const
        {
            return m_matrix;
        }

        /**
         * Get matrix rows
         */
        size_t rows() const
        {
            return m_rows;
        }

        /**
         * Get matrix columns
         */
        size_t cols() const
        {
            return m_cols;
        }

        /**
         * Function Operator.  Gets the reference to the sub-matrix
         */
        reference_type operator()( size_t row, size_t col )
        {
            return child()( row+m_row, col+m_col );
        }

        /**
         * Function Operator.  Gets the reference to the submatrix
         */
        const_reference_type operator()( size_t row, size_t col ) const
        {
            return child()( row+m_row, col+m_col );
        }
        
        /**
         * Get the beginning of the sub-matrix
         */
        iter_t begin()
        {
            return iter_t( *this, 0, 0 );
        }
        
        /**
         * Get the beginning of the sub-matrix (const)
         */
        const_iter_t begin() const
        {
            return const_iter_t( *this, 0, 0 );
        }
        
        /**
         * Get the end of the sub-matrix
         */
        iter_t end()
        {
            return iter_t( *this, rows(), 0 );
        }
        
        /**
         * Get the end of the sub-matrix (const)
         */
        const_iter_t end() const
        {
            return const_iter_t( *this, rows(), 0 );
        }

    private:

        MatrixT&   m_matrix;
        size_t     m_row;
        size_t     m_col;
        size_t     m_rows;
        size_t     m_cols;

}; // End of Sub_Matrix Class

/**
 * Extract a submatrix, i.e. a matrix block.
 */
template <typename MatrixT>
Sub_Matrix<MatrixT> submatrix( Matrix_Base<MatrixT>& matrix,
                              size_t                 row,
                              size_t                 col,
                              size_t                 rows,
                              size_t                 cols )
{
    return Sub_Matrix<MatrixT>( matrix.impl(),
                                row,
                                col,
                                rows,
                                cols );
}

/**
 * @brief Extract a submatrix, i.e. a matrix block (const overlaod).
 */
template <typename MatrixT>
Sub_Matrix<const MatrixT> submatrix( const Matrix_Base<MatrixT>& matrix,
                                     size_t                      row,
                                     size_t                      col,
                                     size_t                      rows,
                                     size_t                      cols )
{
    return Sub_Matrix<const MatrixT>( matrix.impl(),
                                      row,
                                      col,
                                      rows,
                                      cols );
}

} // End of tmns::math