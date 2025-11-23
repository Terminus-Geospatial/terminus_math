/**
 * @file    Vector_Proxy.hpp
 * @author  Marvin Smith
 * @date    10/14/2023
 */
#pragma once

// Terminus Libraries
#include "Vector.hpp"

namespace tmns::math {

/**
 * @class VectorProxy<ElementT,SizeN>
 * A fixed-dimension vector proxy class, treating an arbitrary block of memory as a Vector.
 */
template <typename ElementT,
          size_t   SizeN = 0>
class Vector_Proxy : public Vector_Base<Vector_Proxy<ElementT,SizeN>>
{    
    public:
    
        using value_type = ElementT;
        
        using reference_type = ElementT&;
        
        using const_reference_type = ElementT const&;

        using iter_t = ElementT*;

        using const_iter_t = const ElementT*;

        /**
         * Constructs a vector proxy.
         */
        Vector_Proxy( ElementT *ptr )
            : m_ptr( ptr ) {}

        /**
         * Standard copy assignment operator.
         */
        Vector_Proxy& operator = ( const Vector_Proxy& v )
        {
            if( v.impl().size() != size() )
            {
                std::stringstream sout;
                sout << "Vector must have dimension " << size() << " in vector proxy assignment.";
                throw std::runtime_error( sout.str() );
            }
            Vector_<value_type> tmp( v );
            std::copy( tmp.begin(), tmp.end(), begin() );
            return *this;
        }

        /**
         * Generalized assignment operator, from arbitrary VW vector expressions.
         */
        template <class T>
        Vector_Proxy& operator = ( const Vector_Base<T>& v )
        {
            if( v.impl().size() != size() )
            {
                std::stringstream sout;
                sout << "Vector must have dimension " << size() << " in vector proxy assignment.";
                throw std::runtime_error( sout.str() );
            }
            Vector_<value_type> tmp( v );
            std::copy( tmp.begin(), tmp.end(), begin() );
            return *this;
        }

        /**
         * Temporary-free generalized assignment operator, from arbitrary vector expressions.
         * This is a performance-optimizing function to be used with caution!
         */
        template <typename T>
        Vector_Proxy& operator = ( const Vector_No_Tmp<T>& v )
        {
            if( v.impl().size() != size() )
            {
                std::stringstream sout;
                sout << "Vector must have dimension " << size() << " in vector proxy assignment.";
                throw std::runtime_error( sout.str() );
            }
            std::copy( v.impl().begin(), v.impl().end(), begin() );
            return *this;
        }

        /**
         * Returns the size of the vector.
         */
        size_t size() const
        {
            return SizeN;
        }

        /**
         * Function Operator
        */
        reference_type operator()( size_t i )
        {
            return m_ptr[i];
        }

        /**
         * Function Operator
         */
        const_reference_type operator()( size_t i ) const
        {
            return m_ptr[i];
        }

        /**
         * Index Operator
         */
        reference_type operator[]( size_t i )
        {
            return m_ptr[i];
        }

        /**
         * Index Operator
         */
        const_reference_type operator[]( size_t i ) const
        {
            return m_ptr[i];
        }

        /**
         * Get X Value
         */
        reference_type x() requires ( SizeN >= 1 )
        {
            return m_ptr[0];
        }

        /**
         * Get X Value
         */
        const_reference_type x() const requires ( SizeN >= 1 )
        {
            return m_ptr[0];
        }

        /**
         * Get the Y Value
         */
        reference_type y() requires (SizeN >= 1)
        {
            return m_ptr[1];
        }

        /**
         * Get the Y Value
         */
        const_reference_type y() const requires (SizeN >= 1)
        {
            return m_ptr[1];
        }
        
        /**
         * Get the Z Value
         */
        reference_type z() requires (SizeN >= 2)
        {
            return m_ptr[2];
        }

        /**
         * Get the Z Value
         */
        const_reference_type z() const requires (SizeN >= 2)
        {
            return m_ptr[2];
        }

        /**
         * Beginning iterator position
         */
        iter_t begin()
        {
            return m_ptr;
        }

        /**
         * Beginning iterator position
         */
        const_iter_t begin() const
        {
            return m_ptr;
        }

        /**
         * End iterator position
        */
        iter_t end()
        {
            return m_ptr + size();
        }
        
        /**
         * End iterator position
         */
        const_iter_t end() const
        {
            return m_ptr + size();
        }

    private:

        ElementT* m_ptr { nullptr };

}; // End of Vector_Proxy class

/**
 * Size Info
*/
template <typename ElementT,
          size_t   SizeN>
struct Vector_Size<Vector_Proxy<ElementT,SizeN>>
{
    const static size_t value = SizeN;
};

/**
 * An arbitrary-dimension vector proxy class.
 */
template <typename ElementT>
class Vector_Proxy<ElementT,0> : public Vector_Base<Vector_Proxy<ElementT>>
{
    public:
        using value_type = ElementT;

        using reference_type = ElementT&;

        using const_reference_type = ElementT const&;

        using iter_t = ElementT*;
        
        using const_iter_t = const ElementT*;

        /**
         * Constructs a vector from a buffer
         */
        Vector_Proxy( size_t    size,
                      ElementT* ptr )
            : m_ptr( ptr ),
              m_size( size ) {}

        /**
         * Standard copy assignment operator.
         */
        Vector_Proxy& operator = ( const Vector_Proxy& v )
        {
            if( v.impl().size() != size() )
            {
                std::stringstream sout;
                sout << "Vector must have dimension " << size() << " in vector proxy assignment.";
                throw std::runtime_error( sout.str() );
            }
            Vector_<value_type> tmp( v );
            std::copy( tmp.begin(), tmp.end(), begin() );
            return *this;
        }

        /**
         * Generalized assignment operator, from arbitrary vector expressions.
         */
        template <typename ValueT>
        Vector_Proxy& operator = ( const Vector_Base<ValueT>& v )
        {
            if( v.impl().size() != size() )
            {
                std::stringstream sout;
                sout << "Vector must have dimension " << size() << " in vector proxy assignment.";
                throw std::runtime_error( sout.str() );
            }
            Vector_<value_type> tmp( v );
            std::copy( tmp.begin(), tmp.end(), begin() );
            return *this;
        }

        /**
         * Temporary-free generalized assignment operator, from arbitrary
         * vector expressions.
         * This is a performance-optimizing function to be used with caution!
         */
        template <typename ValueT>
        Vector_Proxy& operator = ( const Vector_No_Tmp<ValueT>& v )
        {
            if( v.impl().size() != size() )
            {
                std::stringstream sout;
                sout << "Vector must have dimension " << size() << " in vector proxy assignment.";
                throw std::runtime_error( sout.str() );
            }
            std::copy( v.impl().begin(), v.impl().end(), begin() );
            return *this;
        }

        /**
         * Returns the size of the vector.
         */
        size_t size() const
        {
            return m_size;
        }

        /**
         * Function Operator
         */
        reference_type operator()( size_t i )
        {
            return m_ptr[i];
        }

        /**
         * Function Operator
         */
        const_reference_type operator()( size_t i ) const
        {
            return m_ptr[i];
        }

        /**
         * Indexing Iterator
         */
        reference_type operator[]( size_t i )
        {
            return m_ptr[i];
        }

        /**
         * Const Indexing Operator
         */
        const_reference_type operator[]( size_t i ) const
        {
            return m_ptr[i];
        }

        /**
         * Get the beginning iterator
         */
        iter_t begin()
        {
            return m_ptr;
        }

        /**
         * Get the beginning iterator
         */
        const_iter_t begin() const
        {
            return m_ptr;
        }

        /**
         * Get the ending iterator
         */
        iter_t end()
        {
            return m_ptr + size();
        }

        /**
         * Get the ending iterator
         */
        const_iter_t end() const
        {
            return m_ptr + size();
        }

    private:

        ElementT* m_ptr { nullptr };
        
        size_t m_size { 0 };

}; // End of Vector_Proxy<>

/**
 * Shallow proxy view of an block of memory as a vector.
 */
template <typename DataT>
Vector_Proxy<DataT>
    vector_proxy( DataT* data,
                  size_t size )
{
    return Vector_Proxy<DataT>( data, size );
}

} // End of tmns::math namespace