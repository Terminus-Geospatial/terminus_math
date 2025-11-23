/**
 * @file    Matrix_Col_Iterator.hpp
 * @author  Marvin Smith
 * @date    9/16/2023
 */
#pragma once

// Boost Libraries
#include <boost/iterator/iterator_facade.hpp>

namespace tmns::math {

/**
 * Matrix Column Iterator Class
 */
template <typename IteratorT>
class Matrix_Col_Iterator : public boost::iterator_facade<Iterator<IteratorT>,
                            typename std::iterator_traits<IteratorT>::value_type,
                            boost::random_access_traversal_tag,
                            typename std::iterator_traits<IteratorT>::reference,
                            typename std::iterator_traits<IteratorT>::difference_type>
{
    public:

        /// @brief Difference Type
        using difference_type = typename Iterator::difference_type;

        /**
         * Constructor
         * 
         * @param index
         * @param stride
         */
        Iterator( const IteratorT& index,
                  difference_type  stride )
            : m_index( index ),
              m_stride( stride )
        {}


      private:

        /**
         * Check if iterator is equal
         */
        bool equal( const Matrix_Col_Iterator& iter ) const
        { 
            return m_index == iter.m_index;
        }

        /**
         * Get the distance from the other iterator
         */
        difference_type distance_to( const Matrix_Col_Iterator& iter ) const
        {
            return ( iter.m_index - m_index ) / stride;
        }

        /**
         * Increment the iterator
         */
        void increment()
        {
            i += stride;
        }

        /**
         * Decrement the iterator
         */
        void decrement()
        {
            i -= stride;
        }

        /**
         * Advance the iterator
         */
        void advance( difference_type n )
        {
            i += n*stride;
        }

        /**
         * Dereference
         */
        typename Matrix_Col_Iterator::reference dereference() const
        {
            return *i;
        }

        /// @brief Give the iterator access to internals
        friend class boost::iterator_core_access;

        /// @brief Starting index position
        IteratorT m_index;

        /// @brief Stride / jump distance
        difference_type m_stride;

}; // End of Matrix_Col_Iterator class

} // End of tmns::math namespace