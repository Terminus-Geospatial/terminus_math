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
 * @file    matrix_row.hpp
 * @author  Marvin Smith
 * @date    9/16/2023
*/
#pragma once

// Terminus Math Libraries
#include <terminus/math/vector/vector_base.hpp>
#include <terminus/math/vector/vectorn.hpp>
#include <terminus/math/vector/vector_utilities.hpp>
#include <terminus/math/matrix/matrix_traits.hpp>

// C++ Libraries
#include <vector>

namespace tmns::math {


/**
 * A matrix row specification with Vector-like semantics.
 */
template <typename MatrixT>
class Matrix_Row : public Vector_Base<Matrix_Row<MatrixT>>
{
    public:

        /// @brief Value Type
        using value_type = typename MatrixT::value_type;

        /// @brief Reference Type
        using reference_type = std::conditional_t<std::is_const_v<MatrixT>,
                                                  typename MatrixT::const_reference_type,
                                                  typename MatrixT::reference_type>;

        /// @brief Const Reference Type
        using const_reference_type = typename MatrixT::const_reference_type;

        /// @brief Iterator Type
        using iter_t = std::conditional_t<std::is_const_v<MatrixT>,
                                          typename MatrixT::const_iter_t,
                                          typename MatrixT::iter_t>;

        /// @brief Const Iterator Type
        using const_iter_t = typename MatrixT::const_iter_t;

        /**
         * Constructor, given reference matrix and row id
         * @param matrix
        */
        Matrix_Row( MatrixT& matrix,
                    size_t   row )
            : m_matrix( matrix ),
              m_row( row )
        {
        }

        /**
         * Standard Copy Assignment Operator
         */
        Matrix_Row& operator=( const Matrix_Row& v )
        {
            // Make sure the size matches
            if( v.size() != size() )
            {
                std::cerr << "Vectors must have the same size in matrix row assignments."
                           << " Using smaller of two for copy.";
            }

            VectorN<value_type> tmp( v );
            std::copy( tmp.begin(), tmp.end(), begin() );
            return *this;
        }

        /**
         * Generalized Assignment Operator
         */
        template <class OtherT>
        Matrix_Row& operator = ( const Vector_Base<OtherT>& v )
        {
            // Make sure size is accurate
            if( v.impl().size() != size() )
            {
                std::cerr << "Vectors must be size size.  Expected: " << size() << ", Actual: "
                          << v.impl().size() << ".  Using smaller of two for copy.";
            }
            VectorN<value_type> tmp( v );
            std::copy( tmp.begin(), tmp.end(), begin() );
            return *this;
        }

        /*
        // For the transposed vctor object
        template <class OtherT>
        Matrix_Row& operator=( VectorTranspose<OtherT> const& v ) {
            VW_ASSERT( v.size() == size(), ArgumentErr() << "Vectors must have same size in matrix row assignment.");
            std::copy( v.begin(), v.end(), begin() );
            return *this;
        }

        /// Temporary-free generalized assignment operator, from arbitrary VW vector expressions.
        /// This is a performance-optimizing function to be used with caution!
        template <class OtherT>
        MatrixRow& operator=( VectorNoTmp<OtherT> const& v ) {
            VW_ASSERT( v.impl().size()==size(), ArgumentErr() << "Vectors must have same size in matrix row assignment." );
            std::copy( v.impl().begin(), v.impl().end(), begin() );
            return *this;
        }*/

        /**
         * Return reference to underlying matrix
         */
        MatrixT& child()
        {
            return m_matrix;
        }

        /**
         * Return const reference to underlying matrix
         */
        MatrixT const& child() const
        {
            return m_matrix;
        }

        /**
         * Get the size of the matrix row
        */
        size_t size() const
        {
            return child().cols();
        }

        /**
         * Function operator, return data at specific row
         */
        reference_type operator()( size_t i )
        {
            return child()( m_row, i );
        }

        /**
         * Const function operator, return data at specific row
         */
        const_reference_type operator()( size_t i ) const
        {
            return child()( m_row, i );
        }

        /**
         * Indexing Operator, return data at specific row
         */
        reference_type operator[]( size_t i )
        {
            return child()( m_row, i );
        }

        /**
         * Const Indexing Operator, return data at specific row
        */
        const_reference_type operator[]( size_t i ) const
        {
            return child()( m_row, i );
        }

        /**
         * Return iterator to front of row
         */
        iter_t begin()
        {
            return child().begin() + m_row * child().cols();
        }

        /**
         * Return const iterator to front of row
         */
        const_iter_t begin() const
        {
            return child().begin() + m_row * child().cols();
        }

        /**
         * Return iterator to end of row
         */
        iter_t end()
        {
            return child().begin() + ( m_row + 1 ) * child().cols();
        }

        /**
         * Return const iterator to front of row
         */
        const_iter_t end() const
        {
            return child().begin() + ( m_row + 1 ) * child().cols();
        }

        /**
         * Get the name
         */
        static std::string name()
        {
            std::stringstream sout;
            sout << "Matrix_Row (" << MatrixT::name() << ")";
            return sout.str();
        }

        /**
         * Print to string
        */
        std::string to_string() const
        {
            std::stringstream sout;
            sout << name() << " (Row: " << m_row << ", Size: " << size() << "): ";
            for( size_t i = 0; i < size(); i++ )
            {
                if( i != 0 )
                {
                    sout << ", ";
                }
                sout << this->operator()(i);
            }
            return sout.str();
        }

    private:

        /// @brief  Handle to parent matrix
        MatrixT& m_matrix;

        /// @brief  Row identifier
        size_t m_row;

}; // End of Matrix_Row class

} // End of tmns::math namespace