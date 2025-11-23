/**
 * @file    XYZ_to_Lon_Lat_Rad_Estimate_Functor.hpp
 * @author  Marvin Smith
 * @date    10/15/2023
 */
#pragma once

// Terminus Libraries
#include <terminus/math/types/Functors.hpp>
#include <terminus/math/vector.hpp>

namespace tmns::coordinate::vw {

/**
 * XYZ to LLA Conversion
 * 
 * @Note: These functions are estimations, they do not produce
 * accurate results. They seem to assume the datum to be spherical.
 * 
 * GCC to GDC conversion with elevation being distance from 0,0,0
 */
class XYZ_to_Lon_Lat_Rad_Estimate_Functor : public math::Unary_Return_Same_Type
{
    public:
        
        /**
         * Constructor
        */
        XYZ_to_Lon_Lat_Rad_Estimate_Functor( bool east_positive    = true,
                                             bool centered_on_zero = true )
            : m_east_positive( east_positive ),
              m_centered_on_zero( centered_on_zero ) {}

        /**
         * Function Operator
         */
        template <typename PointT>
        Result<PointT> operator()( const PointT& p ) const
        {
            return this->apply( p, m_east_positive, m_centered_on_zero );
        }

        /**
         * Apply method to convert individual points
         * Designed to be bound to another functor which applies this to a bulk dataset.
         */
        template <typename PointT>
        static inline Result<PointT> apply( const PointT& p,
                                            bool          east_positive    = true,
                                            bool          centered_on_zero = true )
        {
            // Deal with "missing pixels"
            if( p.magnitude() < 0.0001 )
            {
                return outcome::fail( core::error::ErrorCode::UNINITIALIZED,
                                      "Uninitialized input coordinate." );
            }

            double radius = p.magnitude();
            double sin_lat = p.z() / radius;
            double lat = std::asin( sin_lat );

            double lon;
            if( east_positive )
            {
                lon = std::atan2( p.y(), p.x() );
            }
            else
            {
                // West positive longitude
                lon = std::atan2( -p.y(), p.x() );
            }

            // For consistency-sake, we always return a longitude
            // in the range +/-180.
            if( centered_on_zero )
            {
                if( lon > M_PI )
                {
                    lon -= 2*M_PI;
                }
                if (lon < -M_PI )
                {
                    lon += 2 * M_PI;
                }
            }
            else
            {
                if ( lon < 0 )
                {
                    lon += 2 * M_PI;
                }
                if ( lon > 2 * M_PI )
                {
                    lon -= 2 * M_PI;
                }
            }

            return outcome::ok<PointT>( PointT( { lon * 180.0 / M_PI,
                                                  lat * 180.0 / M_PI,
                                                  radius } ) );
        }

        bool m_east_positive;
        
        bool m_centered_on_zero; // Use the range [-180,180] otherwise [0,360]

}; // End of XYZ_to_Lon_Lat_Rad_Estimate_Functor class

} // End of tmns::coordinate::vw namespace