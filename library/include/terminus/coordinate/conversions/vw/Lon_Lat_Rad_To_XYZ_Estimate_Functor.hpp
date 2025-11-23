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
 * @file    lon_lat_rad_to_xyz_estimate_functor.hpp
 * @author  Marvin Smith
 * @date    10/15/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/math/types/functors.hpp>
#include <terminus/math/vector.hpp>

namespace tmns::coordinate::vw {

/**
 * GDC to GCC conversion with elevation being distance from 0,0,0
 */
class Lon_Lat_Rad_To_XYZ_Estimate_Functor : public math::Unary_Return_Same_Type
{
    public:

        Lon_Lat_Rad_To_XYZ_Estimate_Functor( bool east_positive = true )
            : m_east_positive( east_positive ) {}

        /**
         * Convert from lon, lat, radius to x,y,z:
         *
         * East positive:
         * x = r * cos(latitude) * cos(longitude)
         * y = r * cos(latitude) * sin(longitude)
         * z = r * sin(latitude)
         *
         * West positive:
         * x = r * cos(latitude) * cos(-longitude)
         * y = r * cos(latitude) * sin(-longitude)
         * z = r * sin(latitude)
         */
        template <typename PointT>
        PointT operator()( const PointT& p ) const
        {
            return this->apply( p, m_east_positive );
        }

        /**
         * Perform the conversion
         */
        template <typename PointT>
        static inline Result<PointT> apply( const PointT& p,
                                            bool          east_positive = true )
        {
            typename PointT::value_type z = p(2) * std::sin( p( 1 ) * M_PI / 180.0 );
            typename PointT::value_type sqrt_x_sqr_plus_y_sqr = p(2) * std::cos( p(1) * M_PI / 180.0 );

            if( east_positive )
            {
                return outcome::ok<PointT>( PointT( { sqrt_x_sqr_plus_y_sqr * std::cos(p(0)*M_PI/180.0),
                                                      sqrt_x_sqr_plus_y_sqr * std::sin(p(0)*M_PI/180.0),
                                                      z } ) );
            }
            else
            {
                return outcome::ok<PointT>( PointT( { sqrt_x_sqr_plus_y_sqr * std::cos(-p(0)*M_PI/180.0),
                                                      sqrt_x_sqr_plus_y_sqr * std::sin(-p(0)*M_PI/180.0),
                                                      z } ) );
            }
        }

    private:

            bool m_east_positive;

}; // ENd of Lon_Lat_Rad_to_XYZ_Estimate_Functor class

} // End of tmns::coordinate::vw namespace