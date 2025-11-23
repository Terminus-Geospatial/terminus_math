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
 * @file    indexing_matrix_iterator.hpp
 * @author  Marvin Smith
 * @date    9/19/2023
 */
#pragma once

// C++ Libraries
#include <type_traits>

// Boost Libraries
#include <boost/iterator/iterator_facade.hpp>

namespace tmns::math {

/**
 * @class Indexing_Matrix_Iterator<MatrixT>
 *
 * A general-purpose matrix iterator type.
 *
 * An iterator for an arbitrary matrix type that iterates over the elements of the matrix
 * in the standard (row-major) order.  It keeps track of the element indices, dereferencing
 * via the function call operator.
 */
template <typename MatrixT>
class Indexing_Matrix_Iterator : public boost::iterator_facade<Indexing_Matrix_Iterator<MatrixT>,
                                        std::conditional_t<std::is_const_v<MatrixT>,
                                                           const typename MatrixT::value_type,
                                                           typename MatrixT::value_type>,
                                        boost::random_access_traversal_tag,
                                        std::conditional_t<std::is_const_v<MatrixT>,
                                                           typename MatrixT::const_reference_type,
                                                           typename MatrixT::reference_type>>
{
    public:

        /// @brief Pointer Difference Type
        using difference_type = typename Indexing_Matrix_Iterator::difference_type;

        /**
         * Constructor
         */
        Indexing_Matrix_Iterator( MatrixT&        matrix,
                                  difference_type row,
                                  difference_type col )
            : m_matrix( &matrix ),
              m_row( row ),
              m_col( col )
        {}

    private:

        friend class boost::iterator_core_access;

        /**
         * This has to be a pointer and not a reference because we need to support
         * operator=, and references cannot be reseated.
         */
        MatrixT* m_matrix;

        size_t m_row, m_col;

        /**
         * Check if another iterator is at the same position
        */
        bool equal( const Indexing_Matrix_Iterator& iter ) const
        {
            return ( m_row == iter.m_row &&
                     m_col == iter.m_col );
        }

        /**
         * Check the distance to another iterator
         */
        difference_type distance_to( const Indexing_Matrix_Iterator& iter ) const
        {
            difference_type coldiff = (iter.m_col>m_col) ? difference_type(iter.m_col-m_col) : -difference_type(m_col-iter.m_col);
            difference_type rowdiff = (iter.m_row>m_row) ? difference_type(iter.m_row-m_row) : -difference_type(m_row-iter.m_row);
            return coldiff + rowdiff * m_matrix->cols();
        }

        /**
         * Move the iterator to the next position
         */
        void increment()
        {
            if( ++m_col == m_matrix->cols() )
            {
                m_col=0;
                ++m_row;
            }
        }

        /**
         * Move the iterator to the previous position
         */
        void decrement()
        {
            if( m_col==0 )
            {
                m_col = m_matrix->cols()-1;
                --m_row;
            }
            else
            {
                --m_col;
            }
        }

        /**
         * Advance the iterator N positions
         */
        void advance( difference_type n )
        {
            // This safeguards against suprious division by zero troubles encountered
            // on some platforms when performing operations on degenerate matrices.
            if( m_matrix->cols() == 0 )
            {
                return;
            }
            if( n < 0 )
            {
                difference_type rowdiff = 1 + (-n)/m_matrix->cols();
                m_row -= rowdiff;
                n += rowdiff * m_matrix->cols();
            }
            m_col += n;
            m_row += m_col / m_matrix->cols();
            m_col %= m_matrix->cols();
        }

        /**
         * Dereference the iterator
        */
        typename Indexing_Matrix_Iterator::reference dereference() const
        {
            return (*m_matrix)( m_row, m_col );
        }

}; // End of Indexing_Matrix_Iterator class


} // End of tmns::math namespace