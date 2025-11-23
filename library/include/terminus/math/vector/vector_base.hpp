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
 * @file    vector_base.hpp
 * @author  Marvin Smith
 * @date    9/16/2023
*/
#pragma once

// Terminus Libraries
#include "vector_traits.hpp"

// C++ Libraries
#include <iomanip>

namespace tmns::math {

/**
 * A CRTP base class for vectors and vector expressions.
 * Provides a mechanism for restricting function arguments to
 * vectors, and provides the various arithmetic assignment operators.
 */
template <typename VectorT>
class Vector_Base
{
    public:

        /**
         * Get the derived implementation type
         */
        VectorT& impl()
        {
            return *static_cast<VectorT*>(this);
        }

        /**
         * Get the const derived implementation type
        */
        VectorT const& impl() const
        {
            return *static_cast<VectorT const*>(this);
        }

        /**
         * Addition / Assignment Operator
         */
        template <typename T>
        VectorT&  operator += ( const T& v )
        {
            return impl() = impl() + v;
        }

        /**
         * Subtraction / Assignment Operator
         */
        template <typename T>
        VectorT& operator -= ( const T& v )
        {
            return impl() = impl() - v;
        }

        /**
         * Product / Assignment Operator
         */
        template <typename T>
        VectorT& operator *= ( const T& s )
        {
            return impl() = impl() * s;
        }

        /**
         * Division / Assignment Operator
         */
        template <typename T>
        VectorT& operator /= ( const T& s )
        {
            return impl() = impl() / s;
        }

        /**
         * Set all elements in the vector
         */
        template <typename ElementT>
        void fill( ElementT value )
        {
            impl().fill( value );
        }

        /**
         * Print the matrix in a pretty format
         */
        std::string to_log_string( size_t offset = 0, int precision = 4 ) const
        {
            std::stringstream sout;
            std::string gap( offset, ' ' );
            sout << gap << VectorT::name() << " (size: " << impl().size() << "): ";
            for( size_t r = 0; r < impl().size(); r++ )
            {
                if( r > 0 )
                {
                    sout << ",  ";
                }
                sout << std::fixed << std::setprecision( precision ) << impl().operator()( r );
            }

            return sout.str();
        }

}; // End of Vector_Base class

} // end of tmns::math namespace