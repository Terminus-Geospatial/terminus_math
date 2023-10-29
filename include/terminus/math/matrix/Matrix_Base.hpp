/**
 * @file    Matrix_Base.hpp
 * @author  Marvin Smith
 * @date    9/16/2023
*/
#pragma once

// Terminus Math Libraries
#include "Matrix_Row.hpp"
#include "Matrix_Traits.hpp"

// C++ Libraries
#include <deque>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace tmns::math {

/**
 * @class Matrix_Base
 * 
 * I'm really torn on using VW's concept of a Matrix
 * or simply making a dynamic double-precision matrix that
 * is super simple to read.  I like how Eigen does things, but
 * they also are template-based, which means I'd have to 
 * expose this to customers.  I really don't think it's realistic
 * to do a template-heavy API beind PIMPL, so it's just going to 
 * be this for now.
 */
template <typename MatrixT>
class Matrix_Base
{
    public:

        /**
         * Get the derived matrix type
         */
        MatrixT& impl()
        {
            return *static_cast<MatrixT*>(this);
        }

        /**
         * Get the derived const matrix type
         */
        MatrixT const& impl() const
        {
            return *static_cast<MatrixT const*>(this);
        }

        /**
         * Matrix Addition Assignment
         */
        template <typename T> 
        MatrixT& operator += ( const T& m )
        {
            return impl() = impl() + m;
        }
        
        /**
         * Matrix Subtraction Assignment
         */
        template <typename T>
        MatrixT& operator -= ( const T& m )
        {
            return impl() = impl() - m;
        }  
        
        /**
         * Matrix Multiplication Assignment
         */
        template <typename T>
        MatrixT& operator*=( const T& s )
        {
            return impl() = impl() * s;
        }

        /**
         * Matrix Division Assignment
         */
        template <typename T>
        MatrixT& operator /= ( const T& s )
        {
            return impl() = impl() / s;
        }

        /**
         * Access an individual matrix row, for further access using a second
         * operator[].
         */
        Matrix_Row<MatrixT> operator[]( size_t row )
        {
            return Matrix_Row<MatrixT>( impl(), row );
        }

        /**
         * Access an individual matrix row, for further access using a second
         * operator[].
         */
        Matrix_Row<const MatrixT> operator[]( size_t row ) const
        {
            return Matrix_Row<const MatrixT>( impl(), row );
        }

        /**
         * Print the matrix in a pretty format
         */
        std::string to_log_string( size_t offset = 0, int precision = 4 ) const
        {
            // Solve for the largest string
            int max_val_len = 0;
            std::stringstream temp;
            std::deque<std::string> str_list;
            for( auto it = impl().begin(); it != impl().end(); it++ )
            {
                temp.clear();
                temp.str("");
                temp << std::fixed << std::setprecision(precision) << (*it);
                str_list.push_back( temp.str() );
                max_val_len = temp.str().size();
            }

            // Print the actual matrix
            std::stringstream sout;
            std::string gap( offset, ' ' );
            sout << gap << MatrixT::name() << ": (r: " << impl().rows() << ", c: " << impl().cols() << ")" << std::endl;
            for( size_t r = 0; r < impl().rows(); r++ )
            {
                sout << gap << "|  ";
                for( size_t c = 0; c < impl().cols(); c++ )
                {
                    std::stringstream temp;
                    temp << std::setw( max_val_len ) << std::setfill( ' ' ) << str_list.front();
                    str_list.pop_front();
                    if( c > 0 )
                    {
                        sout << ",  ";
                    }
                    sout << temp.str();
                }
                sout << "  |" << std::endl;
            }
            return sout.str();
        }

}; // End of Matrix_Base class

} // End of tmns::math namespace