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
 * @file    ecef.cpp
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
#include <terminus/geo/coord/ecef.hpp>

namespace tmns::geo::coord {

/****************************************/
/*             Create                   */
/****************************************/
Result<ECEF> ECEF::create( double       x_m,
                           double       y_m,
                           double       z_m,
                           const Datum& horizontal_datum )
{
    if( !std::isfinite( x_m ) || !std::isfinite( y_m ) || !std::isfinite( z_m ) )
    {
        return outcome::fail( error::Error_Code::INVALID_INPUT,
                                "ECEF components must be finite" );
    }

    // Reject positions buried inside the ellipsoid interior.
    const double magnitude = std::sqrt( x_m * x_m + y_m * y_m + z_m * z_m );
    if( magnitude < horizontal_datum.semi_minor_axis() )
    {
        return outcome::fail( error::Error_Code::OUT_OF_BOUNDS,
                                "ECEF position lies inside the ellipsoid" );
    }

    return outcome::ok<ECEF>( ECEF( x_m, y_m, z_m, horizontal_datum ) );
}

/****************************************/
/*             Clone                    */
/****************************************/
std::unique_ptr<Base> ECEF::clone() const
{
    return std::make_unique<ECEF>( *this );
}

/****************************************/
/*           To String                  */
/****************************************/
std::string ECEF::to_string( const std::string& format ) const
{
    std::ostringstream sout;
    if( format == "pretty" )
    {
        sout << "ECEF x: " << x() << ", y: " << y() << ", z: " << z();
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
Result<double> ECEF::distance_to( const ECEF& other ) const
{
    return distance( *this, other );
}

/****************************************/
/*            Distance                  */
/****************************************/
Result<double> ECEF::distance( const ECEF& coord1,
                               const ECEF& coord2 )
{
    if( coord1.horizontal_datum() != coord2.horizontal_datum() )
    {
        return outcome::fail( error::Error_Code::INVALID_CONFIGURATION,
                                "Horizontal datums must match for distance calculation" );
    }

    const double dx = coord2.x() - coord1.x();
    const double dy = coord2.y() - coord1.y();
    const double dz = coord2.z() - coord1.z();
    return outcome::ok<double>( std::sqrt( dx * dx + dy * dy + dz * dz ) );
}

} // end of tmns::geo::coord namespace
