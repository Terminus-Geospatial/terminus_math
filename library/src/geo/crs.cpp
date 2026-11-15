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
 * @file    crs.cpp
 * @author  Marvin Smith
 * @date    11/15/2026
*/

// Terminus Libraries
#include <terminus/geo/crs.hpp>

namespace tmns::geo {


/****************************************/
/*            from_epsg                 */
/****************************************/
Result<CRS> CRS::from_epsg( int code )
{
    // EPSG:4326 — WGS-84 geographic (lon/lat, degrees)
    if( code == 4326 )
    {
        return outcome::ok<CRS>( wgs84() );
    }

    // EPSG:4978 — WGS-84 geocentric (ECEF X/Y/Z, metres)
    if( code == 4978 )
    {
        return outcome::ok<CRS>( ecef() );
    }

    // EPSG:32601–32660 — WGS-84 / UTM zones 1N–60N
    if( code >= 32601 && code <= 32660 )
    {
        return utm( code - 32600, true );
    }

    // EPSG:32701–32760 — WGS-84 / UTM zones 1S–60S
    if( code >= 32701 && code <= 32760 )
    {
        return utm( code - 32700, false );
    }
    return outcome::fail( error::Error_Code::NOT_IMPLEMENTED,
                          "EPSG code requires a registry or backend lookup" );
}


/****************************************/
/*             from_wkt                 */
/****************************************/
Result<CRS> CRS::from_wkt( std::string wkt )
{
    if( wkt.empty() )
    {
        return outcome::fail( error::Error_Code::INVALID_INPUT,
                              "WKT definition is empty" );
    }
    CRS crs;

    // Best-effort classification from WKT keywords; the definition is
    // stored opaquely until a parsing backend exists.  WKT2 "GEODCRS"
    // covers both geographic and geocentric — the "GEOCCS" keyword or
    // Cartesian axes distinguish the geocentric case.
    if( wkt.find( "COMPOUNDCRS" ) != std::string::npos )
    {
        crs.m_kind = CRS_Kind::COMPOUND;
    }
    else if( wkt.find( "PROJCRS" ) != std::string::npos ||
             wkt.find( "PROJCS" ) != std::string::npos )
    {
        crs.m_kind = CRS_Kind::PROJECTED;
    }
    else if( wkt.find( "VERTCRS" ) != std::string::npos ||
             wkt.find( "VERT_CS" ) != std::string::npos )
    {
        crs.m_kind = CRS_Kind::VERTICAL;
    }
    else if( wkt.find( "ENGCRS" ) != std::string::npos ||
             wkt.find( "LOCAL_CS" ) != std::string::npos )
    {
        crs.m_kind = CRS_Kind::ENGINEERING;
    }
    else if( wkt.find( "GEOCCS" ) != std::string::npos )
    {
        crs.m_kind = CRS_Kind::GEOCENTRIC;
    }
    else if( wkt.find( "GEOGCS" ) != std::string::npos ||
             wkt.find( "GEOGCRS" ) != std::string::npos ||
             wkt.find( "GEODCRS" ) != std::string::npos )
    {
        crs.m_kind = CRS_Kind::GEOGRAPHIC;
    }

    crs.m_wkt = std::move( wkt );
    return outcome::ok<CRS>( crs );
}


/****************************************/
/*              wgs84                   */
/****************************************/
CRS CRS::wgs84()
{
    CRS crs;
    crs.m_kind      = CRS_Kind::GEOGRAPHIC;
    crs.m_authority = "EPSG";
    crs.m_code      = 4326;
    crs.m_name      = "WGS 84";
    crs.m_datum     = Datum::WGS84();
    crs.m_axes      = Axis_Set::lon_lat_degrees();
    return crs;
}


/****************************************/
/*               utm                    */
/****************************************/
Result<CRS> CRS::utm( int zone, bool northern )
{
    if( zone < 1 || zone > 60 )
    {
        return outcome::fail( error::Error_Code::INVALID_INPUT,
                              "UTM zone must be in [1,60]" );
    }
    CRS crs;
    crs.m_kind      = CRS_Kind::PROJECTED;
    crs.m_authority = "EPSG";
    crs.m_code      = ( northern ? 32600 : 32700 ) + zone;
    crs.m_name      = "WGS 84 / UTM zone " + std::to_string( zone ) +
                      ( northern ? "N" : "S" );
    crs.m_datum     = Datum::WGS84();
    crs.m_axes      = Axis_Set::easting_northing_meters();
    crs.m_projection_method = "transverse_mercator";
    crs.m_projection_params = {
        { "zone",             static_cast<double>( zone ) },
        { "central_meridian", zone * 6.0 - 183.0            },
        { "scale_factor",     0.9996                        },
        { "false_easting",    500000.0                      },
        { "false_northing",   northern ? 0.0 : 10000000.0   }
    };
    return outcome::ok<CRS>( crs );
}


/****************************************/
/*               ecef                   */
/****************************************/
CRS CRS::ecef()
{
    CRS crs;
    crs.m_kind      = CRS_Kind::GEOCENTRIC;
    crs.m_authority = "EPSG";
    crs.m_code      = 4978;
    crs.m_name      = "WGS 84 (geocentric)";
    crs.m_datum     = Datum::WGS84();
    crs.m_axes      = Axis_Set::ecef_meters();
    return crs;
}


/****************************************/
/*            engineering               */
/****************************************/
CRS CRS::engineering( std::string name, Axis_Set axes )
{
    CRS crs;
    crs.m_kind = CRS_Kind::ENGINEERING;
    crs.m_name = std::move( name );
    crs.m_axes = std::move( axes );
    return crs;
}


/****************************************/
/*               kind                   */
/****************************************/
CRS_Kind CRS::kind() const
{
    return m_kind;
}


/****************************************/
/*               name                   */
/****************************************/
const std::string& CRS::name() const
{
    return m_name;
}


/****************************************/
/*             authority                */
/****************************************/
const std::string& CRS::authority() const
{
    return m_authority;
}


/****************************************/
/*               code                   */
/****************************************/
int CRS::code() const
{
    return m_code;
}


/****************************************/
/*           is_geographic              */
/****************************************/
bool CRS::is_geographic() const
{
    return m_kind == CRS_Kind::GEOGRAPHIC;
}


/****************************************/
/*           is_projected               */
/****************************************/
bool CRS::is_projected() const
{
    return m_kind == CRS_Kind::PROJECTED;
}


/****************************************/
/*           is_geocentric              */
/****************************************/
bool CRS::is_geocentric() const
{
    return m_kind == CRS_Kind::GEOCENTRIC;
}


/****************************************/
/*           has_vertical               */
/****************************************/
bool CRS::has_vertical() const
{
    return m_vertical_datum.has_value() || m_kind == CRS_Kind::VERTICAL ||
           m_kind == CRS_Kind::COMPOUND;
}


/****************************************/
/*               datum                  */
/****************************************/
const std::optional<Datum>& CRS::datum() const
{
    return m_datum;
}


/****************************************/
/*          vertical_datum              */
/****************************************/
const std::optional<Vertical_Datum>& CRS::vertical_datum() const
{
    return m_vertical_datum;
}


/****************************************/
/*               axes                   */
/****************************************/
const Axis_Set& CRS::axes() const
{
    return m_axes;
}


/****************************************/
/*         projection_method            */
/****************************************/
const std::string& CRS::projection_method() const
{
    return m_projection_method;
}


/****************************************/
/*         projection_params            */
/****************************************/
const std::map<std::string, double>& CRS::projection_params() const
{
    return m_projection_params;
}


/****************************************/
/*                wkt                   */
/****************************************/
const std::string& CRS::wkt() const
{
    return m_wkt;
}


/****************************************/
/*            operator==                */
/****************************************/
bool CRS::operator==( const CRS& other ) const
{
    if( !m_authority.empty() && !other.m_authority.empty() )
    {
        return m_authority == other.m_authority && m_code == other.m_code;
    }
    return m_kind == other.m_kind && m_name == other.m_name &&
           m_wkt == other.m_wkt;
}


} // end of tmns::geo namespace
