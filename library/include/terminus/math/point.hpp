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
 * @file    point.hpp
 * @author  Marvin Smith
 * @date    7/12/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/math/enums.hpp>
#include <terminus/math/vector/vector.hpp>

// C++ Libraries
#include <sstream>

namespace tmns::math {

/**
 * N-dimensional point class
 *
 * This differs from the vector in that it's designed for geometric
 * operations, whereas the vector is more geared towards linear algebra
 * and trigonametric operations.
*/
template <typename ValueT, int Dims>
class Point_ : public Vector_<ValueT,Dims>
{
    public:

        typedef ValueT value_type;

        /**
         * Default Constructor
        */
        Point_() = default;

        /**
         * Parameterized Constructor
        */
        Point_( std::array<ValueT,Dims> data )
          : Vector_<ValueT,Dims>( std::move(data) )
        {
        }

        /**
         * Copy-Constructor
         */
        template <typename OtherVectorT>
        Point_( const Vector_Base<OtherVectorT>& other )
        {
            // Make sure the size matches
            if( other.impl().size() != Dims )
            {
                std::stringstream sout;
                sout << "Input vector has incorrect size. Expected: " << Dims
                     << ", Actual: " << other.impl().size();
                throw std::runtime_error( sout.str() );
            }

            for( size_t i = 0; i < this->data().size(); i++ )
            {
                this->data()[i] = other.impl()[i];
            }
        }

        /**
         * Assignment Operator
         */
        template <typename OtherVectorT>
        Point_<ValueT,Dims> operator = ( const Vector_Base<OtherVectorT>& other )
        {
            // Make sure the size matches
            if( other.impl().size() != Dims )
            {
                std::stringstream sout;
                sout << "Input vector has incorrect size. Expected: " << Dims
                     << ", Actual: " << other.impl().size();
                throw std::runtime_error( sout.str() );
            }

            // Create output point
            Point_<ValueT,Dims> output;
            for( size_t i = 0; i < Dims; i++ )
            {
                output[i] = other[i];
            }
            return output;
        }

        /**
         * Addition Operator
        */
        template< typename OtherValueT>
        Point_<ValueT,Dims> operator + ( const Point_<OtherValueT,Dims>& rhs ) const
        {
            Point_<ValueT,Dims> result;
            for( size_t i=0; i < this->data().size(); i++ )
            {
                result.data()[i] = this->data()[i] + rhs.data()[i];
            }
            return result;
        }

        /**
         * Addition Operator
        */
        template< typename OtherValueT>
        Point_<ValueT,Dims> operator + ( const Vector_<OtherValueT,Dims>& rhs ) const
        {
            Point_<ValueT,Dims> result;
            for( size_t i=0; i < this->data().size(); i++ )
            {
                result.data()[i] = this->data()[i] + rhs.data()[i];
            }
            return result;
        }

        /**
         * Addition assignment operator
        */
        template <typename OtherT>
        Point_<ValueT,Dims>& operator += ( const Point_<OtherT,Dims>& rhs )
        {
            for( size_t i = 0; i < this->data().size(); i++ )
            {
                this->data()[i] += rhs.data()[i];
            }
            return (*this);
        }

        /**
         * Subtraction assignment operator
        */
        template <typename OtherT>
        Point_<ValueT,Dims>& operator -= ( const Point_<OtherT,Dims>& rhs )
        {
            for( size_t i = 0; i < this->data().size(); i++ )
            {
                this->data()[i] -= rhs.data()[i];
            }
            return (*this);
        }

        /**
         * Subtraction Operator
        */
        template< typename OtherValueT>
        Point_<ValueT,Dims> operator - ( const Point_<OtherValueT,Dims>& rhs ) const
        {
            Point_<ValueT,Dims> result;
            for( size_t i=0; i < this->data().size(); i++ )
            {
                result.data()[i] = this->data()[i] - rhs.data()[i];
            }
            return result;
        }

        /**
         * Subtraction Operator
        */
        template< typename OtherValueT>
        Point_<ValueT,Dims> operator - ( const Vector_<OtherValueT,Dims>& rhs ) const
        {
            Point_<ValueT,Dims> result;
            for( size_t i=0; i < this->data().size(); i++ )
            {
                result.data()[i] = this->data()[i] - rhs.data()[i];
            }
            return result;
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
         * Return the element-wise min of N points
         *
         * Really F-ing cool solution:
         *  https://stackoverflow.com/a/63330289/2142228
        */
        template< typename    Point1,
                  typename    Point2,
                  typename... Tail>
        static constexpr Point1 elementwise_min( const Point1&  point1,
                                                 const Point2&  point2,
                                                 const Tail&... tail )
        {
            if constexpr (sizeof...(tail) == 0)
            {
                Point1 result;
                for( size_t i=0; i < result.size(); i++ )
                {
                    result[i] = std::min( point1[i],
                                          static_cast<typename Point1::value_type>( point2[i] ) );
                }
                return std::move( result );
            }
            else
            {
                return elementwise_min( elementwise_min( point1,
                                                         point2 ),
                                        tail... );
            }
        }

        /**
         * Return the element-wise max of N points
        */
        template< typename    Point1,
                  typename    Point2,
                  typename... Tail>
        static constexpr Point1 elementwise_max( const Point1&  point1,
                                                 const Point2&  point2,
                                                 const Tail&... tail )
        {
            if constexpr (sizeof...(tail) == 0)
            {
                Point1 result;
                for( size_t i=0; i < result.size(); i++ )
                {
                    result[i] = std::max( point1[i],
                                          static_cast<typename Point1::value_type>( point2[i] ) );
                }
                return std::move( result );
            }
            else
            {
                return elementwise_max( elementwise_max( point1,
                                                         point2 ),
                                        tail... );
            }
        }

        /**
         * Compute the distance between the 2 points
         *
         * @param point1 - First point
         * @param point2 - Second point (Doesn't need to be the same value-type)
         * @param method - L1 or L2
         *
         * L1:  https://en.wikipedia.org/wiki/Taxicab_geometry
         * L2:  Distance Formula
        */
        template <typename OtherValueT>
        static constexpr double distance( const Point_<ValueT,Dims>&      point1,
                                          const Point_<OtherValueT,Dims>& point2,
                                          DistanceType                    method = DistanceType::L2 )
        {
            double dist = 0;

            // Taxicab distance
            if( method == DistanceType::L1 )
            {
                auto res = point1 - point2;
                for( size_t i = 0; i < Dims; i++ )
                {
                    dist += std::fabs( res[i] );
                }
            }
            else if( method == DistanceType::L2 )
            {
                dist = ( point1 - point2 ).magnitude();
            }
            return dist;
        }

        /**
         * Print to string
        */
        std::string to_string() const
        {
            std::stringstream sout;
            sout << "Point (Dims: " << this->size() << "): ";
            for( size_t i = 0; i < this->size(); i++ )
            {
                if( i != 0 )
                {
                    sout << ", ";
                }
                sout << this->operator[](i);
            }
            return sout.str();
        }

}; // End of Point class

// Alias for Point2_ object
template <typename ValueT> using Point2_ = Point_<ValueT,2>;
template <typename ValueT> using Point3_ = Point_<ValueT,3>;
template <typename ValueT> using Point4_ = Point_<ValueT,4>;

// Double-Precision Points
using Point2d = Point2_<double>;
using Point3d = Point3_<double>;
using Point4d = Point4_<double>;

// Single-Precision Points
using Point2f = Point2_<float>;
using Point3f = Point3_<float>;
using Point4f = Point4_<float>;

// Integer Points
using Point2i = Point2_<int>;
using Point3i = Point3_<int>;
using Point4i = Point4_<int>;

using Point2l = Point2_<int64_t>;
using Point3l = Point3_<int64_t>;
using Point4l = Point4_<int64_t>;


} // End of tmns::math