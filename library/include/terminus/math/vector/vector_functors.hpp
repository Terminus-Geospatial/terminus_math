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
 * @file    Vector_Functors.hpp
 * @author  Marvin Smith
 * @date    10/15/2023
 */
#pragma once

// Terminus Libraries
#include <terminus/math/vector/vector_base.hpp>

// C++ Libraries
#include <type_traits>

// Boost Libraries
#include <boost/iterator/iterator_facade.hpp>

namespace tmns::math {

/**
 * A unary elementwise vector function class.
 */
template <typename VectorT,
          typename FunctorT>
class Vector_Unary_Functor : public Vector_Base<Vector_Unary_Functor<VectorT,FunctorT>>
{
    public:

        using value_type = std::invoke_result_t<FunctorT,typename VectorT::value_type>;

        using reference_type = value_type;

        using const_reference_type = value_type;

        class Iterator : public boost::iterator_facade<Iterator,
                                                       value_type,
                                                       boost::random_access_traversal_tag,
                                                       reference_type>
        {
            public:

                /**
                 * Iterator Constructor
                */
                Iterator( const typename VectorT::const_iter_t& i,
                          const FunctorT&                       func )
                    : m_iter( i ),
                      m_functor( func ) {}

            private:

                friend class boost::iterator_core_access;

                bool equal( const Iterator& iter ) const
                {
                    return m_iter == iter.m_iter;
                }

                typename Iterator::difference_type distance_to( const Iterator& iter ) const
                {
                    return iter.m_iter - m_iter;
                }

                void increment()
                {
                    ++m_iter;
                }

                void decrement()
                {
                    --m_iter;
                }

                void advance( typename Iterator::difference_type n )
                {
                    m_iter += n;
                }

                typename Iterator::reference dereference() const
                {
                    return m_functor( *m_iter );
                }

                typename VectorT::const_iter_t m_iter;
                FunctorT m_functor;

        }; // End of Iterator Class

        using iter_t       = Iterator;
        using const_iter_t = Iterator;

        Vector_Unary_Functor( const VectorT& v )
            : m_vector(v) {}

        template <typename Arg1>
        Vector_Unary_Functor( const VectorT& v, Arg1 a1 )
            : m_vector(v), m_functor( a1 ) {}

        VectorT const& child() const
        {
            return m_vector;
        }

        size_t size() const
        {
            return child().size();
        }

        const_reference_type operator()( size_t i ) const
        {
            return m_functor( child()(i) );
        }

        const_reference_type operator[]( size_t i ) const
        {
            return m_functor( child()(i) );
        }

        iter_t begin() const
        {
            return iter_t( child().begin(), m_functor );
        }

        iter_t end() const
        {
            return iter_t( child().end(), m_functor );
        }

        /**
         * Get name
         */
        static std::string name()
        {
            std::stringstream sout;
            sout << "Vector_Unary_Functor<" << VectorT::name() << "," << FunctorT::name() << ">";
            return sout.str();
        }

    private:

        VectorT const& m_vector;

        FunctorT m_functor;

}; // End of Vector_Unary_Functor class

template <typename VectorT,
          typename FunctorT>
struct Vector_Size<Vector_Unary_Functor<VectorT,FunctorT>>
{
    static const size_t value = Vector_Size<VectorT>::value;
};

/**
 * A binary elementwise vector function class.
 */
template <typename Vector1T,
          typename Vector2T,
          typename FunctorT>
class Vector_Binary_Functor : public Vector_Base<Vector_Binary_Functor<Vector1T,Vector2T,FunctorT>>
{
    public:

        // Underlying Data Type
        using value_type = std::invoke_result_t<FunctorT,
                                                typename Vector1T::value_type,
                                                typename Vector2T::value_type>;

        // Reference Type
        using reference_type = value_type;

        // Const Reference Type
        using const_reference_type = value_type;

        /**
         * Overloaded iterator
         */
        class Iterator : public boost::iterator_facade<Iterator,
                                                       value_type,
                                                       boost::random_access_traversal_tag,
                                                       reference_type>
        {
            public:

                Iterator( const typename Vector1T::const_iter_t& i1,
                          const typename Vector2T::const_iter_t& i2,
                          const FunctorT&                        func )
                    : m_i1( i1 ),
                      m_i2( i2 ),
                      m_func( func ) {}

            private:

                // Give boost access
                friend class boost::iterator_core_access;

                bool equal( const Iterator& iter ) const
                {
                    return ( m_i1 == iter.m_i1 ) && ( m_i2 == iter.m_i2 );
                }

                typename Iterator::difference_type distance_to( const Iterator& iter ) const
                {
                    return iter.m_i1 - m_i1;
                }

                void increment()
                {
                    ++m_i1;
                    ++m_i2;
                }

                void decrement()
                {
                    --m_i1;
                    --m_i2;
                }

                void advance( typename Iterator::difference_type n )
                {
                    m_i1 += n;
                    m_i2 += n;
                }

                typename Iterator::reference dereference() const
                {
                    return m_func( *m_i1, *m_i2 );
                }

                typename Vector1T::const_iter_t m_i1;
                typename Vector2T::const_iter_t m_i2;
                FunctorT m_func;
        };

        /**
         * Constructor
        */
        Vector_Binary_Functor( const Vector1T& v1,
                               const Vector2T& v2 )
            : m_vec1( v1 ),
              m_vec2( v2 ),
              m_func()
        {
            if( v1.size() != v2.size() )
            {
                std::stringstream sout;
                sout << "Vectors are not the same size. Actual: " << v1.size() << " vs " << v2.size();
                throw std::runtime_error( sout.str() );
            }
        }

        template <typename Argument1>
        Vector_Binary_Functor( const Vector1T& v1,
                               const Vector2T& v2,
                               Argument1       a1 )
            : m_vec1( v1 ),
              m_vec2( v2 ),
              m_func( a1 )
        {
            if( v1.size() != v2.size() )
            {
                std::stringstream sout;
                sout << "Vectors are not the same size. Actual: " << v1.size() << " vs " << v2.size();
                throw std::runtime_error( sout.str() );
            }
        }

        Vector1T const& child1() const
        {
            return m_vec1;
        }

        Vector2T const& child2() const
        {
            return m_vec2;
        }

        size_t size() const
        {
            return child1().size();
        }

        const_reference_type operator()( size_t i ) const
        {
            return m_func( child1()(i),
                           child2()(i) );
        }

        const_reference_type operator[]( size_t i ) const
        {
            return m_func( child1()[i],
                           child2()[i] );
        }

        using iter_t = Iterator;
        using const_iter_t = Iterator;

        /**
         * Get the beginning iterator position
         */
        Iterator begin() const
        {
            return Iterator( child1().begin(),
                             child2().begin(),
                             m_func );
        }

        /**
         * Get the end iterator position
         */
        Iterator end() const
        {
            return Iterator( child1().end(),
                             child2().end(),
                             m_func );
        }

        /**
         * Get name
         */
        static std::string name()
        {
            std::stringstream sout;
            sout << "Vector_Binary_Functor<" << Vector1T::name() << "," << Vector2T::name() << "," << FunctorT::name() << ">";
            return sout.str();
        }

    private:

        const Vector1T& m_vec1;
        const Vector2T& m_vec2;
        FunctorT m_func;

}; // End of Vector_Binary_Functor class

template <typename Vector1T,
          typename Vector2T,
          typename FunctorT>
struct Vector_Size<Vector_Binary_Functor<Vector1T,Vector2T,FunctorT>>
{
    static const size_t value = (Vector_Size<Vector1T>::value!=0)?(Vector_Size<Vector1T>::value):(Vector_Size<Vector2T>::value);
};

} // End of tmns::math namespace