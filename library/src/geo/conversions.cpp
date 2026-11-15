/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/*                                                                                    */
/*                           Copyright (c) 2026 Terminus LLC                          */
/*                                                                                    */
/*                                All Rights Reserved.                                */
/*                                                                                    */
/*          Use of this source code is governed by LICENSE in the repo root.          */
/*                                                                                    */
/***************************# INTELLECTUAL PROPERTY RIGHTS ****************************/
/**
 * @file    conversions.cpp
 * @author  Marvin Smith
 * @date    11/15/2026
*/

// C++ Standard Libraries
#include <cmath>

// Terminus Libraries
#include <terminus/geo/conversions.hpp>
#include <terminus/geo/constants.hpp>

namespace tmns::geo::coord {

namespace {

/// First/second eccentricity squared for a datum.
struct Ellipsoid {
    double a;
    double b;
    double e2;
    double ep2;
};

Ellipsoid ellipsoid( const Datum& datum )
{
    const double a  = datum.semi_major_axis();
    const double f  = datum.flattening();
    const double b  = a * ( 1.0 - f );
    const double e2 = f * ( 2.0 - f );
    return { a, b, e2, e2 / ( 1.0 - e2 ) };
}

/// Meridian arc M(phi) — series through e^6 (standard UTM formulation).
double meridian_arc( const Ellipsoid& el, double phi )
{
    const double e2 = el.e2, e4 = e2 * e2, e6 = e4 * e2;
    return el.a * ( ( 1.0 - e2 / 4.0 - 3.0 * e4 / 64.0 - 5.0 * e6 / 256.0 ) * phi
                  - ( 3.0 * e2 / 8.0 + 3.0 * e4 / 32.0 + 45.0 * e6 / 1024.0 ) * std::sin( 2.0 * phi )
                  + ( 15.0 * e4 / 256.0 + 45.0 * e6 / 1024.0 ) * std::sin( 4.0 * phi )
                  - ( 35.0 * e6 / 3072.0 ) * std::sin( 6.0 * phi ) );
}

constexpr double UTM_K0 = 0.9996;

} // end of anonymous namespace


/****************************************/
/*             to_ecef                  */
/****************************************/
Result<ECEF> to_ecef( const Geographic& geographic )
{
    const auto el = ellipsoid( geographic.horizontal_datum() );
    const double lat = geographic.latitude_radians();
    const double lon = geographic.longitude_radians();
    const double h   = geographic.elevation_meters();

    const double sin_lat = std::sin( lat );
    const double cos_lat = std::cos( lat );
    const double n       = el.a / std::sqrt( 1.0 - el.e2 * sin_lat * sin_lat );

    return ECEF::create( ( n + h ) * cos_lat * std::cos( lon ),
                         ( n + h ) * cos_lat * std::sin( lon ),
                         ( n * ( 1.0 - el.e2 ) + h ) * sin_lat,
                         geographic.horizontal_datum() );
}


/****************************************/
/*         to_geographic (ECEF)         */
/****************************************/
Result<Geographic> to_geographic( const ECEF& ecef )
{
    const auto el = ellipsoid( ecef.horizontal_datum() );
    const double x = ecef.x(), y = ecef.y(), z = ecef.z();
    const double p = std::hypot( x, y );

    if( p < 1.0 )
    {
        // Nearly on the polar axis — longitude undefined; use poles.
        const double lat = std::copysign( M_PI / 2.0, z );
        const double h   = std::fabs( z ) - el.b;
        return Geographic::from_radians( lat, 0.0, h, ecef.horizontal_datum() );
    }

    const double lon = std::atan2( y, x );

    // Iterative latitude/height (Bowring seed, few Newton-style steps).
    double lat = std::atan2( z, p * ( 1.0 - el.e2 ) );
    double h   = 0.0;
    for( int i = 0; i < 8; ++i )
    {
        const double sin_lat = std::sin( lat );
        const double n       = el.a / std::sqrt( 1.0 - el.e2 * sin_lat * sin_lat );
        h   = p / std::cos( lat ) - n;
        const double new_lat = std::atan2( z, p * ( 1.0 - el.e2 * n / ( n + h ) ) );
        if( std::fabs( new_lat - lat ) < 1e-14 )
        {
            lat = new_lat;
            break;
        }
        lat = new_lat;
    }

    return Geographic::from_radians( lat, lon, h, ecef.horizontal_datum() );
}


/****************************************/
/*              to_utm                  */
/****************************************/
Result<UTM> to_utm( const Geographic& geographic, std::int32_t zone )
{
    const bool northern = geographic.latitude_radians() >= 0.0;
    return to_utm( geographic, zone,
                   northern ? Hemisphere::NORTH : Hemisphere::SOUTH );
}


/****************************************/
/*         to_utm (hemisphere)          */
/****************************************/
Result<UTM> to_utm( const Geographic& geographic,
                    std::int32_t      zone,
                    Hemisphere        hemisphere )
{
    if( zone == 0 )
    {
        zone = UTM::zone_for_longitude( geographic.longitude_radians() );
    }

    const auto el    = ellipsoid( geographic.horizontal_datum() );
    const double lat = geographic.latitude_radians();
    const double lon = geographic.longitude_radians();
    const double lam = lon - UTM::central_meridian( zone );

    const double sin_lat = std::sin( lat );
    const double cos_lat = std::cos( lat );
    const double tan_lat = std::tan( lat );

    const double n = el.a / std::sqrt( 1.0 - el.e2 * sin_lat * sin_lat );
    const double t = tan_lat * tan_lat;
    const double c = el.ep2 * cos_lat * cos_lat;
    const double a = lam * cos_lat;
    const double m = meridian_arc( el, lat );

    const double easting =
        UTM_K0 * n * ( a
                     + ( 1.0 - t + c ) * a * a * a / 6.0
                     + ( 5.0 - 18.0 * t + t * t + 72.0 * c - 58.0 * el.ep2 )
                         * std::pow( a, 5 ) / 120.0 )
        + 500000.0;

    const double northing_base =
        UTM_K0 * ( m + n * tan_lat *
                   ( a * a / 2.0
                   + ( 5.0 - t + 9.0 * c + 4.0 * c * c ) * std::pow( a, 4 ) / 24.0
                   + ( 61.0 - 58.0 * t + t * t + 600.0 * c - 330.0 * el.ep2 )
                       * std::pow( a, 6 ) / 720.0 ) );

    const bool       northern = hemisphere == Hemisphere::NORTH;
    const double     northing = northern ? northing_base : northing_base + 10000000.0;

    return UTM::create( zone,
                        hemisphere,
                        easting,
                        northing,
                        geographic.elevation_meters(),
                        geographic.horizontal_datum() );
}


/****************************************/
/*         to_geographic (UTM)          */
/****************************************/
Result<Geographic> to_geographic( const UTM& utm )
{
    const auto el = ellipsoid( utm.horizontal_datum() );

    const double x = utm.easting_meters() - 500000.0;
    double       y = utm.northing_meters();
    if( utm.hemisphere() == Hemisphere::SOUTH )
    {
        y -= 10000000.0;
    }

    // Footpoint latitude via the inverse meridian-arc series.
    const double m  = y / UTM_K0;
    const double mu = m / ( el.a * ( 1.0 - el.e2 / 4.0 - 3.0 * el.e2 * el.e2 / 64.0
                                    - 5.0 * el.e2 * el.e2 * el.e2 / 256.0 ) );

    const double e1 = ( 1.0 - std::sqrt( 1.0 - el.e2 ) ) / ( 1.0 + std::sqrt( 1.0 - el.e2 ) );
    const double e1_2 = e1 * e1, e1_3 = e1_2 * e1, e1_4 = e1_2 * e1_2;

    const double phi1 = mu
        + ( 3.0 * e1 / 2.0 - 27.0 * e1_3 / 32.0 ) * std::sin( 2.0 * mu )
        + ( 21.0 * e1_2 / 16.0 - 55.0 * e1_4 / 32.0 ) * std::sin( 4.0 * mu )
        + ( 151.0 * e1_3 / 96.0 ) * std::sin( 6.0 * mu )
        + ( 1097.0 * e1_4 / 512.0 ) * std::sin( 8.0 * mu );

    const double sin1 = std::sin( phi1 );
    const double cos1 = std::cos( phi1 );
    const double tan1 = std::tan( phi1 );

    const double n1 = el.a / std::sqrt( 1.0 - el.e2 * sin1 * sin1 );
    const double r1 = el.a * ( 1.0 - el.e2 ) /
                      std::pow( 1.0 - el.e2 * sin1 * sin1, 1.5 );
    const double t1 = tan1 * tan1;
    const double c1 = el.ep2 * cos1 * cos1;
    const double d  = x / ( n1 * UTM_K0 );

    const double d2 = d * d;
    const double lat = phi1 - ( n1 * tan1 / r1 ) *
        ( d2 / 2.0
        - ( 5.0 + 3.0 * t1 + 10.0 * c1 - 4.0 * c1 * c1 - 9.0 * el.ep2 )
            * d2 * d2 / 24.0
        + ( 61.0 + 90.0 * t1 + 298.0 * c1 + 45.0 * t1 * t1
            - 252.0 * el.ep2 - 3.0 * c1 * c1 )
            * d2 * d2 * d2 / 720.0 );

    const double lon = UTM::central_meridian( utm.zone() ) +
        ( d - ( 1.0 + 2.0 * t1 + c1 ) * d * d2 / 6.0
            + ( 5.0 - 2.0 * c1 + 28.0 * t1 - 3.0 * c1 * c1
                + 8.0 * el.ep2 + 24.0 * t1 * t1 )
                * d * d2 * d2 / 120.0 ) / cos1;

    return Geographic::from_radians( lat, lon, utm.elevation_meters(),
                                     utm.horizontal_datum() );
}


} // end of tmns::geo::coord namespace
