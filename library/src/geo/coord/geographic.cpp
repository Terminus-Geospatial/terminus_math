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
 * @file    geographic.cpp
 * @author  Marvin Smith
 * @date    10/26/2026
*/

// C++ Standard Libraries
#include <cmath>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

// Terminus Libraries
#include <terminus/geo/coord/geographic.hpp>

namespace tmns::geo::coord {

/****************************************/
/*             Create                   */
/****************************************/
Result<Geographic> Geographic::create( double       lon_rad,
                                       double       lat_rad,
                                       double       elev_m,
                                       const Datum& horizontal_datum )
{
    if( !std::isfinite( lon_rad ) || !std::isfinite( lat_rad ) ||
        !std::isfinite( elev_m ) )
    {
        return outcome::fail( error::Error_Code::INVALID_INPUT,
                                "Coordinate components must be finite" );
    }
    if( lat_rad < -M_PI_2 || lat_rad > M_PI_2 )
    {
        return outcome::fail( error::Error_Code::OUT_OF_BOUNDS,
                                "Latitude out of range [-pi/2, pi/2]" );
    }
    if( lon_rad < -M_PI || lon_rad > M_PI )
    {
        return outcome::fail( error::Error_Code::OUT_OF_BOUNDS,
                                "Longitude out of range [-pi, pi]" );
    }
    return outcome::ok<Geographic>(
        Geographic( lon_rad, lat_rad, elev_m, horizontal_datum ) );
}

/****************************************/
/*           From Degrees               */
/****************************************/
Result<Geographic> Geographic::from_degrees( double       lat_deg,
                                             double       lon_deg,
                                             double       elev_m,
                                             const Datum& horizontal_datum )
{
    if( !std::isfinite( lat_deg ) || !std::isfinite( lon_deg ) ||
        !std::isfinite( elev_m ) )
    {
        return outcome::fail( error::Error_Code::INVALID_INPUT,
                                "Coordinate components must be finite" );
    }
    if( lat_deg < -90.0 || lat_deg > 90.0 )
    {
        return outcome::fail( error::Error_Code::OUT_OF_BOUNDS,
                                "Latitude out of range [-90, 90]" );
    }
    if( lon_deg < -180.0 || lon_deg > 180.0 )
    {
        return outcome::fail( error::Error_Code::OUT_OF_BOUNDS,
                                "Longitude out of range [-180, 180]" );
    }
    return outcome::ok<Geographic>(
        Geographic( lon_deg * DEG_TO_RAD,
                    lat_deg * DEG_TO_RAD,
                    elev_m,
                    horizontal_datum ) );
}

/****************************************/
/*           From Radians               */
/****************************************/
Result<Geographic> Geographic::from_radians( double       lat_rad,
                                             double       lon_rad,
                                             double       elev_m,
                                             const Datum& horizontal_datum )
{
    return create( lon_rad, lat_rad, elev_m, horizontal_datum );
}

/****************************************/
/*             Clone                    */
/****************************************/
std::unique_ptr<Base> Geographic::clone() const
{
    return std::make_unique<Geographic>( *this );
}

/****************************************/
/*           To String                  */
/****************************************/
std::string Geographic::to_string( const std::string& format ) const
{
    std::ostringstream sout;
    if( format == "pretty" )
    {
        sout << "Geographic lat: " << latitude_degrees()
             << ", lon: "  << longitude_degrees()
             << ", elev: " << elevation_meters();
    }
    else if( format == "google" )
    {
        sout << latitude_degrees() << ", " << longitude_degrees();
    }
    else
    {
        throw std::invalid_argument( "Unsupported format: " + format );
    }
    return sout.str();
}

/****************************************/
/*           Distance To                */
/****************************************/
Result<double> Geographic::distance_to( const Geographic& other,
                                        Distance_Method   method ) const
{
    return distance( *this, other, method );
}

/****************************************/
/*            Distance                  */
/****************************************/
Result<double> Geographic::distance( const Geographic& coord1,
                                     const Geographic& coord2,
                                     Distance_Method   method )
{
    if( coord1.horizontal_datum() != coord2.horizontal_datum() )
    {
        return outcome::fail( error::Error_Code::INVALID_CONFIGURATION,
                                "Horizontal datums must match for distance calculation" );
    }

    switch( method )
    {
        case Distance_Method::HAVERSINE:
            return haversine_distance( coord1, coord2 );
        case Distance_Method::VINCENTY:
            return vincenty_distance( coord1, coord2 );
    }
    return outcome::fail( error::Error_Code::INVALID_INPUT,
                            "Unknown distance method" );
}

/****************************************/
/*        Haversine Distance            */
/****************************************/
Result<double> Geographic::haversine_distance( const Geographic& coord1,
                                               const Geographic& coord2 )
{
    const double radius = coord1.m_horizontal_datum.semi_major_axis() != 6378137.0
        ? coord1.m_horizontal_datum.semi_major_axis()
        : 6371000.0;  // Mean Earth radius for WGS84

    const double lat1 = coord1.latitude_radians();
    const double lat2 = coord2.latitude_radians();
    const double dlat = lat2 - lat1;
    const double dlon = coord2.longitude_radians() - coord1.longitude_radians();

    const double a = std::sin( dlat / 2.0 ) * std::sin( dlat / 2.0 ) +
                     std::cos( lat1 ) * std::cos( lat2 ) *
                     std::sin( dlon / 2.0 ) * std::sin( dlon / 2.0 );
    const double c = 2.0 * std::asin( std::sqrt( a ) );

    return outcome::ok<double>( radius * c );
}

/****************************************/
/*         Vincenty Distance            */
/****************************************/
Result<double> Geographic::vincenty_distance( const Geographic& coord1,
                                              const Geographic& coord2 )
{
    const double a = coord1.m_horizontal_datum.semi_major_axis();
    const double f = coord1.m_horizontal_datum.flattening();
    const double b = coord1.m_horizontal_datum.semi_minor_axis();

    const double lat1 = coord1.latitude_radians();
    const double lat2 = coord2.latitude_radians();
    const double L    = coord2.longitude_radians() - coord1.longitude_radians();

    if( lat1 == lat2 && L == 0.0 )
    {
        return outcome::ok<double>( 0.0 );
    }

    const double U1 = std::atan( ( 1.0 - f ) * std::tan( lat1 ) );
    const double U2 = std::atan( ( 1.0 - f ) * std::tan( lat2 ) );
    const double sin_U1 = std::sin( U1 ), cos_U1 = std::cos( U1 );
    const double sin_U2 = std::sin( U2 ), cos_U2 = std::cos( U2 );

    double lambda = L;
    double sin_sigma = 0.0, cos_sigma = 0.0, sigma = 0.0;
    double sin_alpha = 0.0, cos_sq_alpha = 0.0, cos_2sigma_m = 0.0;

    bool converged = false;
    for( int iter = 0; iter < 200; ++iter )
    {
        const double sin_lambda = std::sin( lambda );
        const double cos_lambda = std::cos( lambda );

        sin_sigma = std::sqrt(
            ( cos_U2 * sin_lambda ) * ( cos_U2 * sin_lambda ) +
            ( cos_U1 * sin_U2 - sin_U1 * cos_U2 * cos_lambda ) *
            ( cos_U1 * sin_U2 - sin_U1 * cos_U2 * cos_lambda ) );

        if( sin_sigma == 0.0 )
        {
            return outcome::ok<double>( 0.0 );
        }

        cos_sigma = sin_U1 * sin_U2 + cos_U1 * cos_U2 * cos_lambda;
        sigma     = std::atan2( sin_sigma, cos_sigma );

        sin_alpha    = cos_U1 * cos_U2 * sin_lambda / sin_sigma;
        cos_sq_alpha = 1.0 - sin_alpha * sin_alpha;

        cos_2sigma_m = cos_sq_alpha != 0.0
            ? cos_sigma - 2.0 * sin_U1 * sin_U2 / cos_sq_alpha
            : 0.0;

        const double C = f / 16.0 * cos_sq_alpha *
                         ( 4.0 + f * ( 4.0 - 3.0 * cos_sq_alpha ) );

        const double lambda_prev = lambda;
        lambda = L + ( 1.0 - C ) * f * sin_alpha *
                 ( sigma + C * sin_sigma *
                   ( cos_2sigma_m + C * cos_sigma *
                     ( -1.0 + 2.0 * cos_2sigma_m * cos_2sigma_m ) ) );

        if( std::abs( lambda - lambda_prev ) < 1e-12 )
        {
            converged = true;
            break;
        }
    }
    if( !converged )
    {
        return outcome::fail( error::Error_Code::CONVERSION_ERROR,
                                "Vincenty formula failed to converge; "
                                "points may be nearly antipodal" );
    }

    const double u_sq = cos_sq_alpha * ( a * a - b * b ) / ( b * b );
    const double A = 1.0 + u_sq / 16384.0 *
                     ( 4096.0 + u_sq * ( -768.0 + u_sq * ( 320.0 - 175.0 * u_sq ) ) );
    const double B = u_sq / 1024.0 *
                     ( 256.0 + u_sq * ( -128.0 + u_sq * ( 74.0 - 47.0 * u_sq ) ) );

    const double delta_sigma = B * sin_sigma * (
        cos_2sigma_m + B / 4.0 * (
            cos_sigma * ( -1.0 + 2.0 * cos_2sigma_m * cos_2sigma_m ) -
            B / 6.0 * cos_2sigma_m * ( -3.0 + 4.0 * sin_sigma * sin_sigma ) *
                ( -3.0 + 4.0 * cos_2sigma_m * cos_2sigma_m ) ) );

    return outcome::ok<double>( b * A * ( sigma - delta_sigma ) );
}

} // end of tmns::geo::coord namespace
