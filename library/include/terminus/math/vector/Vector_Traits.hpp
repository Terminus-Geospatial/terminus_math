/**
 * @file    Vector_Traits.hpp
 * @author  Marvin Smith
 * @date    9/16/2023
*/
#pragma once

// C++ Libraries
#include <vector>

namespace tmns::math {

/**
 * A type function to compute the dimension of a vector expression
 * at compile time (or zero for dynamically-sized vectors).
 */
template <typename VectorT>
struct Vector_Size
{
    const static std::size_t value = 0;
};

/**
 * A wrapper template class for vectors and vector expressions.
 * Provides a mechanism for disabling the use of temporary objects
 * during vector assignment in cases where the user deems it safe.
 */
template <typename VectorT>
class Vector_No_Tmp
{
    public:

        /**
         * Constructor
        */
        Vector_No_Tmp( const VectorT& value )
            : m_value( value )
        {}

        /**
         * Get the underlying vector
        */
        VectorT const& impl() const
        {
            return m_value;
        }

    private:

        /// Underlying Vector
        const VectorT& m_value;

}; // End of Vector_No_Tmp



} // end of tmns::math namespace