/**
 * @file    Size.hpp
 * @author  Marvin Smith
 * @date    7/20/2023
*/
#pragma once

// Terminus Math Libraries
#include "vector/Vector.hpp"

namespace tmns::math {

/**
 * Simple data structure for dimensional sizes info without ranges
 * Rectangle is a good class for size, but it has corners.
*/
template <typename ValueT, int Dims>
class Size_
{
    public:

        /// @brief Value Type
        using value_type = ValueT;

        /// @brief Reference Type
        using reference_type = value_type&;

        /// @brief Const Reference Type
        using const_reference_type = const value_type&;

        /// Default Constructor
        Size_() = default;

        /**
         * Parameterized Constructor
         */
        Size_( const std::array<ValueT,Dims>& data )
         : m_data( data )
        {
        }

        /**
         * @brief Get the width
         */
        const_reference_type width() const
        {
            return m_data.x();
        }

        /**
         * @brief Get the width value
         */
        reference_type width()
        {
            return m_data.x();
        }

        /**
         * @brief Get the height
         */
        const_reference_type height() const requires ( Dims >= 2 )
        {
            return m_data.y();
        }

        /**
         * @brief Get the height value
         */
        reference_type height() requires ( Dims >= 2 )
        {
            return m_data.y();
        }

        /**
         * @brief Get the depth value
         */
        reference_type depth() const requires ( Dims >= 3 )
        {
            return m_data.z();
        }

        /**
         * @brief Get the depth value
         */
        reference_type depth() requires ( Dims >= 3 )
        {
            return m_data.z();
        }

        /**
         * @brief Indexing Operator (Const/Copy)
         */
        const_reference_type operator[]( size_t id ) const
        {
            return m_data[id];
        }

        /**
         * Indexing Operator (Ref)
        */
        reference_type operator[]( size_t id )
        {
            return m_data[id];
        }

        /**
         * Convert to a string
        */
        std::string to_string() const
        {
            std::stringstream sout;
            sout << "Size (Dims: " << Dims << ", Width: " << std::fixed << width() << ", Height: " << height() << ")";
            return sout.str();
        }

        /**
         * Get data as vector
         */
        Vector_<ValueT,Dims> const& as_vector() const
        {
            return m_data;
        }

    private:

        /// Underlying Data Structure
        Vector_<ValueT,Dims> m_data { { 0 } };

}; // End of Size Class

/// Simple Aliases
using Size2i = Size_<int,2>;
using Size3i = Size_<int,3>;
using Size2f = Size_<float,2>;
using Size3f = Size_<float,3>;
using Size2d = Size_<double,2>;
using Size3d = Size_<double,3>;

} // End of tmns::math namespace