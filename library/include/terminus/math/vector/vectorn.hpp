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
 * @file    vectorn.hpp
 * @author  Marvin Smith
 * @date    7/30/2023
*/
#pragma once

// Terminus Math Libraries
#include <terminus/math/vector/vector.hpp>

// C++ Libraries
#include <initializer_list>

namespace tmns::math {

/**
 * @class VectorN
 * N-dimensional, resizable vector
*/
template <typename ValueT>
class Vector_<ValueT,0> : public Vector_Base<Vector_<ValueT>>
{
    public:

        /// @brief Value Type
        using value_type = ValueT;

        /// @brief Array Type
        using array_type = std::vector<value_type>;

        /// @brief Reference Type
        using reference_type = ValueT&;

        /// @brief Const Reference Type
        using const_reference_type = const ValueT&;

        /// @brief Iterator Type
        using iter_t = typename array_type::iterator;

        /// @brief Const Iterator Type
        using const_iter_t = typename array_type::const_iterator;


        /**
         * Default Constructor
         */
        Vector_() = default;

        /**
         * @brief Constructor from any other Vector type.
         */
        template <typename OtherVectorT>
        Vector_( const Vector_Base<OtherVectorT>& v )
            : m_data( v.impl().begin(),
                      v.impl().end() )
        {
        }

        /**
         * @brief Create a zero vector of the specific size
         */
        Vector_( size_t vector_size )
            : m_data( vector_size, 0 )
        {}

        /**
         * @brief Create a vector of the specific size and value
         */
        Vector_( size_t vector_size, ValueT val )
            : m_data( vector_size, val )
        {}

        /**
         * @brief Constructor Given an Array
         */
        Vector_( std::initializer_list<ValueT> data )
          : m_data( data )
        {
        }

        /**
         * @brief Constructor Given an std::array
         */
        template <typename OtherValueT,
                  size_t OtherSize>
        explicit Vector_( const std::array<OtherValueT,OtherSize>& data )
          : m_data( data.begin(), data.end() )
        {
        }

        /**
         * Constructor given pointer and size
        */
        explicit Vector_( const ValueT* ptr,
                          size_t        size )
            : m_data( ptr, ptr + size )
        {
        }

        /**
         * Copy Constructor
         */
        Vector_( const Vector_& other )
            : m_data( other.m_data )
        {
        }

        /**
         * General Assignment Operator
         */
        Vector_& operator = ( const Vector_& v )
        {
            Vector_ temp( v );
            m_data = temp.m_data;
            return (*this);
        }

        /**
         * Assignment operator for any vector type.
         */
        template <typename OtherVectorT>
        Vector_& operator = ( const Vector_Base<OtherVectorT>& v )
        {
            Vector_ tmp( v );
            m_data = tmp.m_data;
            return *this;
        }

        /**
         * Temporary-free generalized assignment operator, from arbitrary vector expressions.
         * This is a performance-optimizing function to be used with caution!
         */
        template <typename T>
        Vector_& operator=( const Vector_No_Tmp<T>& v )
        {
            return *this = v.impl();
        }

        /**
         * Get the current size of the array
        */
        size_t size() const
        {
            return m_data.size();
        }

        /**
         * Clear the array
         */
        void clear()
        {
            m_data.clear();
        }

        /**
         * Index Operator
        */
        const_reference_type operator[]( size_t idx ) const
        {
            return this->m_data[idx];
        }

        /**
         * Index Reference Operator
         */
        reference_type operator[]( size_t idx )
        {
            return this->m_data[idx];
        }

        /**
         * Call Operator
        */
        const_reference_type operator()( size_t idx ) const
        {
            return this->m_data[idx];
        }

        /**
         * Call Reference Operator
         */
        reference_type operator()( size_t idx )
        {
            return this->m_data[idx];
        }

        /**
         * Get copy of internal data at specific index
        */
        const_reference_type at( size_t idx ) const
        {
            return m_data.at(idx);
        }

        /**
         * Get reference to internal data at specific index
        */
        reference_type at( size_t idx )
        {
            return m_data.at(idx);
        }

        /**
         * @brief Get X Value
         */
        const_reference_type x() const
        {
            return m_data[0];
        }

        /**
         * @brief Get X Reference
         */
        reference_type x()
        {
            return m_data[0];
        }

        /**
         * @brief Get Y Value
         */
        const_reference_type y() const
        {
            return m_data[1];
        }

        /**
         * @brief Get Y Reference
         */
        reference_type y()
        {
            return m_data[1];
        }

        /**
         * @brief Get Z Value
         */
        const_reference_type z() const
        {
            return m_data[2];
        }

        /**
         * @brief Get Z Reference
         */
        reference_type z()
        {
            return m_data[2];
        }

        /**
         * @brief Return copy of internal data array
        */
        array_type const& data() const
        {
            return m_data;
        }

        /**
         * @brief Return reference of internal data array
         */
        array_type& data()
        {
            return m_data;
        }

        /**
         * Get the starting iterator position
         */
        iter_t begin()
        {
            return m_data.begin();
        }

        /**
         * Get the starting iterator position
         */
        const_iter_t begin() const
        {
            return m_data.begin();
        }

        /**
         * Get the ending iterator position
         */
        iter_t end()
        {
            return m_data.end();
        }

        /**
         * Get the ending iterator position
         */
        const_iter_t end() const
        {
            return m_data.end();
        }

        /**
         * @brief Add entry to vector
         */
        void push_back( ValueT new_entry )
        {
            m_data.push_back( new_entry );
        }

        /**
         * @brief Set all elements in the vector
         */
        template <typename TP>
        void fill( TP value )
        {
            std::fill( begin(),
                       end(),
                       value );
        }

        /**
         * Normalize the vector
         */
        Vector_<ValueT,0> normalize() const
        {
            Vector_<ValueT,0> output = (*this);
            auto mag = magnitude();
            for( size_t i = 0; i < size(); i++ )
            {
                output[i] /= mag;
            }
            return output;
        }

        /**
         * Compute the vector magnitude
        */
        double magnitude() const
        {
            return std::sqrt( magnitude_sq() );
        }

        /**
         * Compute the square of the magnitude
        */
        double magnitude_sq() const
        {
            double mag = 0;
            for( const auto& elem : this->data() )
            {
                mag += elem * elem;
            }
            return mag;
        }

        /**
         * Perform a Dot-Product
         */
        template <typename Vector1T,
                  typename Vector2T>
        static double dot( const Vector_Base<Vector1T>& vec1,
                           const Vector_Base<Vector2T>& vec2 )
        {
            if( vec1.impl().size() > vec2.impl().size() )
            {
                std::stringstream sout;
                sout << "Vector 1 is smaller than the second vector. Actual: "
                     << vec1.impl().size() << ", " << vec2.impl().size();
                throw std::runtime_error( sout.str() );
            }
            double mag = 0;

            auto it1 = vec1.impl().begin();
            auto it2 = vec2.impl().begin();
            for( ; it1 != vec1.impl().end(); it1++, it2++ )
            {
                mag += (*it1) * (*it2);
            }
            return mag;
        }

        /**
         * Get name
         */
        static std::string name()
        {
            std::stringstream sout;
            sout << "VectorN<" << Data_Type_Name<ValueT>::name() << ",0>";
            return sout.str();
        }

    private:

        std::vector<ValueT> m_data;

}; // End of Vector_<ValueT,0> Class

template <typename ValueT>
using VectorN = Vector_<ValueT,0>;

using VectorNf = VectorN<float>;
using VectorNd = VectorN<double>;

} // End of tmns::math namespace