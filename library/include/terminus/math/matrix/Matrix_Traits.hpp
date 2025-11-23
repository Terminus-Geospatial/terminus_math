/**
 * @file    Matrix_Traits.hpp
 * @author  Marvin Smith
 * @date    9/16/2023
*/
#pragma once

namespace tmns::math {

/**
 * Simple type structure to compute the number of rows of a matrix
 * during compilation (or zero for dynamically-sized vectors).
 */
template <typename MatrixT>
struct Matrix_Rows
{
    const static size_t value = 0;
};

/**
 * Simple type structure to compute the number of columns of a matrix
 * during compilation (or zero for dynamically-sized vectors).
 */
template <typename MatrixT>
struct Matrix_Cols
{
    const static size_t value = 0;
};


/**
 * A wrapper template class for matrices and matrix expressions. Provides
 * a mechanism for disabling the use of temporary objects during matrix assignment
 * in cases where the user deems it safe.
 */
template <typename MatrixT>
class Matrix_No_Tmp
{
    public:

        /**
         * Constructor
         */
        Matrix_No_Tmp( const MatrixT& value )
            : m_value( value )
        {}

        /**
         * Get underlying matrix
         */
        const MatrixT& impl() const
        {
            return m_value;
        }

    private:

        /// @brief Underlying Matrix
        const MatrixT& m_value;
        
}; // End of Matrix_No_Tmp class

} // End of tmns::math namespace