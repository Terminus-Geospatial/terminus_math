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
 * @file    utm.cpp
 * @author  Marvin Smith
 * @date    10/26/2026
*/

// C++ Standard Libraries
#include <cmath>
#include <cstdint>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

// Terminus Libraries
#include <terminus/geo/coord/utm.hpp>

namespace tmns::geo::coord {

/****************************************/
/*             Create                   */
/****************************************/
Result<UTM> UTM::create( std::int32_t zone,
                         Hemisphere   hemisphere,
                         double       easting_m,
                         double       northing_m,
                         double       elev_m,
                         const Datum& horizontal_datum )
{
    if( !std::isfinite( easting_m ) || !std::isfinite( northing_m ) ||
        !std::isfinite( elev_m ) )
    {
        return outcome::fail( error::Error_Code::INVALID_INPUT,
                                "UTM components must be finite" );
    }
    if( zone < 1 || zone > 60 )
    {
        return outcome::fail( error::Error_Code::OUT_OF_BOUNDS,
                                "UTM zone out of range [1, 60]" );
    }
    if( easting_m < UTM::MIN_EASTING || easting_m > UTM::MAX_EASTING )
    {
        return outcome::fail( error::Error_Code::OUT_OF_BOUNDS,
                                "UTM easting out of range" );
    }
    if( northing_m < UTM::MIN_NORTHING || northing_m > UTM::MAX_NORTHING )
    {
        return outcome::fail( error::Error_Code::OUT_OF_BOUNDS,
                                "UTM northing out of range" );
    }
    return outcome::ok<UTM>(
        UTM( zone, hemisphere, easting_m, northing_m, elev_m, horizontal_datum ) );
}

/****************************************/
/*        Zone For Longitude            */
/****************************************/
std::int32_t UTM::zone_for_longitude( double lon_rad )
{
    const double lon_deg = lon_rad * RAD_TO_DEG;
    return static_cast<std::int32_t>( std::floor( ( lon_deg + 180.0 ) / 6.0 ) ) + 1;
}

/****************************************/
/*         Central Meridian             */
/****************************************/
double UTM::central_meridian( std::int32_t zone )
{
    return ( -183.0 + ( zone - 1 ) * 6.0 + 3.0 ) * DEG_TO_RAD;
}

/****************************************/
/*             Clone                    */
/****************************************/
std::unique_ptr<Base> UTM::clone() const
{
    return std::make_unique<UTM>( *this );
}

/****************************************/
/*           To String                  */
/****************************************/
std::string UTM::to_string( const std::string& format ) const
{
    std::ostringstream sout;
    if( format == "pretty" )
    {
        sout << "UTM " << zone() << ( hemisphere() == Hemisphere::NORTH ? "N" : "S" )
             << " easting: " << easting_meters()
             << ", northing: " << northing_meters()
             << ", elev: " << elevation_meters();
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
Result<double> UTM::distance_to( const UTM& other ) const
{
    return distance( *this, other );
}

/****************************************/
/*            Distance                  */
/****************************************/
Result<double> UTM::distance( const UTM& coord1,
                              const UTM& coord2 )
{
    if( coord1.horizontal_datum() != coord2.horizontal_datum() )
    {
        return outcome::fail( error::Error_Code::INVALID_CONFIGURATION,
                                "Horizontal datums must match for distance calculation" );
    }
    if( coord1.zone() != coord2.zone() || coord1.hemisphere() != coord2.hemisphere() )
    {
        return outcome::fail( error::Error_Code::INVALID_INPUT,
                                "Euclidean distance requires the same UTM zone "
                                "and hemisphere" );
    }

    const double de = coord2.easting_meters() - coord1.easting_meters();
    const double dn = coord2.northing_meters() - coord1.northing_meters();
    const double dz = coord2.elevation_meters() - coord1.elevation_meters();
    return outcome::ok<double>( std::sqrt( de * de + dn * dn + dz * dz ) );
}

} // end of tmns::geo::coord namespace
