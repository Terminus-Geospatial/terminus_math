/**
 * @file    Vector_Transpose.hpp
 * @author  Marvin Smith
 * @date    10/14/2023
 */
#pragma once

// Terminus Libraries
#include "VectorN.hpp"

namespace tmns::math {

/**
 * A transposed vector class.  This class represents the transposed version of
 * a vector.  Note that unlike a transposed matrix, a transposed vector is not
 * simply another vector.  For this reason Vector_Transposed does not derive from
 * Vector_Base<>, since that would permit a variety of mathematically meaninless
 * expressions to be created with unexpected results.  An unfortunate side effect
 * of this is that explicit overloads must be provided for all operations on
 * transposed vectors.  For the moment only a limited set of operations are supported,
 * including primarily the mathematical operators.
 */
template <typename VectorT>
class Vector_Transpose
{
    public:
    
        using value_type = typename VectorT::value_type;

        /// @brief Reference Type
        using reference_type = std::conditional_t<std::is_const_v<VectorT>,
                                                  typename VectorT::const_reference_type,
                                                  typename VectorT::reference_type>;
        
        /// @brief Const Reference Type
        using const_reference_type = typename VectorT::const_reference_type;

        /// @brief Iterator type
        using iter_t = std::conditional_t<std::is_const_v<VectorT>,
                                          typename VectorT::const_iter_t,
                                          typename VectorT::iter_t>;
        
        /// @brief Const iterator type
        using const_iter_t = typename VectorT::const_iter_t;

        /**
         * Constructor from other vector type
         */
        explicit Vector_Transpose( VectorT& v )
            : m_vector( v ) {}

        template <typename OtherT>
        Vector_Transpose& operator = ( const Vector_Transpose<OtherT>& v )
        {
            if( v.size() != size() )
            {
                std::stringstream sout;
                sout << "Vectors must have same size in transposed vector assignment";
                throw std::runtime_error( sout.str() );
            }
            m_vector = v.child();
            return *this;
        }

        /**
         * Get underlying vector
         */
        VectorT& child()
        {
            return m_vector;
        }
        
        /**
         * Get underlying vector
         */
        VectorT const& child() const
        {
            return m_vector;
        }

        /**
         * Get vector size
        */
        size_t size() const
        {
            return m_vector.size();
        }

        /**
         * Function operator
         */
        reference_type operator()( size_t i )
        {
            return child()(i);
        }
        
        /**
         * Function operator
         */
        const_reference_type operator()( size_t i ) const
        {
            return child()(i);
        }
        
        /**
         * Index operator
         */
        reference_type operator[]( size_t i )
        {
            return child()[i];
        }

        /**
         * Index operator
         */
        const_reference_type operator[]( size_t i ) const
        {
            return child()[i];
        }

        /**
         * Get beginning iterator position
         */
        iter_t begin()
        {
            return child().begin();
        }

        /**
         * Get beginning iterator position
         */
        const_iter_t begin() const
        {
            return child().begin();
        }

        /**
         * Get end iterator position
         */
        iter_t end()
        {
            return child().end();
        }
        
        /**
         * Get end iterator position
         */
        const_iter_t end() const
        {
            return child().end();
        }
    
    private:

        /**
         * We want to store Vector objects by reference so that we don't copy them,
         * but we want to store everything else by value so that we can return
         * transposed versions of various vector expressions.
         */
        template <typename T>
        struct Vector_Closure
        {
            using type = T;
        };

        template <typename ElementT,
                  size_t   SizeN>
        struct Vector_Closure<Vector_<ElementT,SizeN>>
        {
            using type = Vector_<ElementT,SizeN>&;
        };

        template <typename ElementT,
                  size_t   SizeN>
        struct Vector_Closure<const Vector_<ElementT,SizeN>>
        {
            using type = Vector_<ElementT,SizeN> const&;
        };
        
        /// @brief Underlying Vectors
        typename Vector_Closure<VectorT>::type m_vector;

}; // End of Vector_Transpose class

/**
 * Vector transpose.
 */
template <typename VectorT>
Vector_Transpose<VectorT> transpose( Vector_Base<VectorT>& vector )
{
    return Vector_Transpose<VectorT>( vector.impl() );
}

/**
 * Vector transpose (const overload).
 */
template <typename VectorT>
Vector_Transpose<const VectorT> transpose( const Vector_Base<VectorT>& vector )
{
    return Vector_Transpose<const VectorT>( vector.impl() );
}

/**
 * Vector transpose (transpose overload).
 */
template <typename VectorT>
VectorT& transpose( Vector_Transpose<VectorT>& vector )
{
    return vector.child();
}

/**
 * Vector transpose (const transpose overload).
 */
template <typename VectorT>
VectorT const& transpose( const Vector_Transpose<VectorT>& vector )
{
    return vector.child();
}

} // End of tmns::math namespace