/**
 * @file    Indexing_Vector_Iterator.hpp
 * @author  Marvin Smith
 * @date    10/14/2023
 */
#pragma once

namespace tmns::math {

// *******************************************************************
  // class IndexingVectorIterator<VectorT>
  // A general purpose vector iterator type.
  // *******************************************************************

  template <class VectorT>
  class IndexingVectorIterator : public boost::iterator_facade<IndexingVectorIterator<VectorT>,
    typename boost::mpl::if_<boost::is_const<VectorT>,
    const typename VectorT::value_type,
    typename VectorT::value_type>::type,
    boost::random_access_traversal_tag,
    typename boost::mpl::if_<boost::is_const<VectorT>,
    typename VectorT::const_reference_type,
    typename VectorT::reference_type>::type> {

  public:
    typedef typename IndexingVectorIterator::difference_type difference_type;

    IndexingVectorIterator( VectorT& vector, difference_type index ) :
      m_vector(&vector), m_index(index) {}
  private:
    friend class boost::iterator_core_access;

    // This has to be a pointer and not a reference because we need to support
    // operator=, and references cannot be reseated.
    VectorT* m_vector;
    difference_type m_index;

    bool equal( IndexingVectorIterator const& iter ) const {
      return m_index==iter.m_index;
    }

    difference_type distance_to( IndexingVectorIterator const& iter ) const {
      return difference_type(iter.m_index)-difference_type(m_index);
    }

    void increment() { ++m_index; }
    void decrement() { --m_index; }

    void advance( difference_type n ) {
      if ( m_vector->size() == 0 ) return;
      m_index += n;
    }

    typename IndexingVectorIterator::reference dereference() const {
      return (*m_vector)[m_index];
    }
    
}; // End of Indexing_Vector_Iterator namespace

} // End of tmns::math namespace