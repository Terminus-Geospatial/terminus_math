/**
 * @file    Datum.cpp
 * @author  Marvin Smith
 * @date    9/20/2023
 */
#include <terminus/coordinate/Datum.hpp>

/// Terminus Libraries
#include "Datum_Impl.hpp"

/// Boost Libraries
#include <boost/algorithm/string.hpp>

/// GDAL Libraries
#include <ogr_spatialref.h>

namespace tmns::coordinate {

/********************************/
/*      Default Constructor     */
/********************************/
Datum::Datum()
    : m_impl( std::make_shared<detail::Datum_Impl>() )
{
    OGRSpatialReference gdal_spatial_ref;
    gdal_spatial_ref.importFromProj4( m_impl->m_proj_str.c_str() );
    
    // Datum Name
    const char* datum_name = gdal_spatial_ref.GetAttrValue("DATUM");
    if( datum_name )
    {
        m_impl->m_name = datum_name;
    }

    // Sphere Name
    const char* spheroid_name = gdal_spatial_ref.GetAttrValue("SPHEROID");
    if( spheroid_name )
    {
        m_impl->m_spheroid_name = spheroid_name;
    }

    // Meridian Name
    const char* meridian_name = gdal_spatial_ref.GetAttrValue("PRIMEM");
    if( meridian_name )
    {
        m_impl->m_meridian_name = meridian_name;
    }

    // Semi components
    OGRErr e1, e2;
    double semi_major = gdal_spatial_ref.GetSemiMajor( &e1 );
    double semi_minor = gdal_spatial_ref.GetSemiMinor( &e2 );
    if( e1 != OGRERR_FAILURE && e2 != OGRERR_FAILURE )
    {
        m_impl->set_semi_major_axis( semi_major );
        m_impl->set_semi_minor_axis( semi_minor );
    }

    m_impl->m_meridian_offset = gdal_spatial_ref.GetPrimeMeridian();

    char* proj4_str_tmp;
    gdal_spatial_ref.exportToProj4( &proj4_str_tmp );

    std::string proj_str = proj4_str_tmp;
    CPLFree( proj4_str_tmp );

    m_impl->m_proj_str = proj_str; // The other call can change the string, don't let it!
}

/****************************************/
/*      Parameterized Constructor       */
/****************************************/
Datum::Datum( std::string_view name,
              std::string_view spheroid_name,
              std::string_view meridian_name,
              double           semi_major_axis,
              double           semi_minor_axis,
              double           meridian_offset )
    : m_impl( std::make_unique<detail::Datum_Impl>() )
{ 
    m_impl->m_name            = name;
    m_impl->m_spheroid_name   = spheroid_name;
    m_impl->m_meridian_name   = meridian_name;
    m_impl->m_semi_major_axis = semi_major_axis;
    m_impl->m_semi_minor_axis = semi_minor_axis;
    m_impl->m_meridian_offset = meridian_offset;
}

/************************************/
/*          Get Datum Name          */
/************************************/
const std::string& Datum::name() const
{
    return m_impl->m_name;
}

/***************************************/
/*          Get Spheroid Name          */
/***************************************/
const std::string& Datum::spheroid_name() const
{
    return m_impl->m_spheroid_name;
}

/************************************/
/*        Get Meridian Name         */
/************************************/
const std::string& Datum::meridian_name() const
{
    return m_impl->m_meridian_name;
}

/************************************/
/*       Get Semi Major Axis        */
/************************************/
const double& Datum::semi_major_axis() const
{
    return m_impl->m_semi_major_axis;
}

/************************************/
/*       Get Semi Minor Axis        */
/************************************/
const double& Datum::semi_minor_axis() const
{
    return m_impl->m_semi_minor_axis;
}

/********************************/
/*      Get Meridian Offset     */
/********************************/
const double& Datum::meridian_offset() const
{
    return m_impl->m_meridian_offset;
}

/********************************/
/*      Check if geocentric     */
/********************************/
bool Datum::geocentric() const
{
    return m_impl->m_geocentric;
}

/********************************/
/*      Get Centric Radius      */
/********************************/
double Datum::radius( [[maybe_unused]] double lat_deg,
                      [[maybe_unused]] double lon_deg ) const
{
    // Optimize in the case of spherical datum
    if( m_impl->m_semi_major_axis == 
        m_impl->m_semi_minor_axis )
    {
        return m_impl->m_semi_major_axis;
    }

    // Bi-axial ellpisoid datum
    double a = m_impl->m_semi_major_axis;
    double b = m_impl->m_semi_minor_axis;
    double t = std::atan( ( a / b ) * std::tan( lat_deg * M_PI / 180.0 ) );
    double x = a * std::cos( t );
    double y = b * std::sin( t );
    return std::sqrt( x * x + y * y );
}

/************************************/
/*      From Well-Known Name        */
/************************************/
ImageResult<Datum> Datum::from_well_known_name( const std::string& name )
{
    Datum output;
    output.m_impl->m_meridian_name   = "Greenwich";
    output.m_impl->m_geocentric      = false;
    output.m_impl->m_meridian_offset = 0.0;

    // These numbers will be over-written later. However, we must
    // still initialize them, otherwise when the set_semi_major_axis()
    // function is invoked later it will result in un-initialized
    // variables (since that function sets the semi-major axis but
    // assumes the semi-minor axis is already set).
    output.m_impl->m_semi_major_axis =  6378137;
    output.m_impl->m_semi_minor_axis = 6356752.3142;

    std::string up_name = boost::to_upper_copy( name );

    /// WGS84
    if( up_name == "WGS84"     ||
        up_name == "WGS_1984"  ||
        up_name == "WGS 1984"  ||
        up_name == "WGS1984"   ||
        up_name == "WORLD GEODETIC SYSTEM 1984" ||
        up_name == "EARTH" )
    {
        return from_proj_string( "+proj=longlat +datum=WGS84 +no_defs" );
    }

    /// WGS 1972
    if( up_name == "WGS72" ||
        up_name == "WGS_1972" )
    {
        return from_proj_string( "+proj=longlat +ellps=WGS72 +no_defs" );
    }

    /// North American Datum 1983
    if( up_name == "NAD83" ||
        up_name == boost::to_upper_copy(std::string("North_American_Datum_1983" ) ) )
    {
        return from_proj_string( "+proj=longlat +ellps=GRS80 +datum=NAD83 +no_defs" );
    }

    /// North American Datum 1927
    if( up_name == "NAD27" ||
        up_name == boost::to_upper_copy( std::string( "North_American_Datum_1927" ) ) )
    {
        return from_proj_string( "+proj=longlat +datum=NAD27 +no_defs" );
    }

    /// Lunar Datum
    if( up_name == "D_MOON" ||
        up_name == "MOON" )
    {
        output.m_impl->m_name            = "D_MOON";
        output.m_impl->m_spheroid_name   = "MOON";
        output.m_impl->m_meridian_name   = "Reference Meridian";
        output.m_impl->m_semi_major_axis = output.m_impl->m_semi_minor_axis = 1737400;
        output.m_impl->m_meridian_offset = 0.0;
        output.m_impl->m_proj_str        = "+a=1737400 +b=1737400";
        return outcome::ok<Datum>( std::move( output ) );
    }

    if( up_name == "D_MARS" ||
        up_name == "MARS" )
    {
        output.m_impl->m_name            = "D_MARS";
        output.m_impl->m_spheroid_name   = "MARS";
        output.m_impl->m_meridian_name   = "Reference Meridian";
        output.m_impl->m_semi_major_axis = output.m_impl->m_semi_minor_axis = 3396190;
        output.m_impl->m_meridian_offset = 0.0;
        output.m_impl->m_proj_str        = "+a=3396190 +b=3396190";
        return outcome::ok<Datum>( std::move( output ) );
    }

    if (up_name == "MOLA")
    {
        output.m_impl->m_name            = "D_MARS";
        output.m_impl->m_spheroid_name   = "MARS";
        output.m_impl->m_meridian_name   = "Reference Meridian";
        output.m_impl->m_semi_major_axis = output.m_impl->m_semi_minor_axis = 3396000;
        output.m_impl->m_meridian_offset = 0.0;
        output.m_impl->m_proj_str        = "+a=3396000 +b=3396000";
        return outcome::ok<Datum>( std::move( output ) );
    }

    return outcome::fail( core::error::ErrorCode::NOT_FOUND,
                          "No datum found matching name '", name, "'" );
}

/********************************************/
/*          From OGR Spatial Ref            */
/********************************************/
ImageResult<Datum> Datum::from_ogr_spatial_ref( const OGRSpatialReference& gdal_spatial_ref )
{
    Datum result;

    // Datum Name
    const char* datum_name = gdal_spatial_ref.GetAttrValue("DATUM");
    if( datum_name )
    {
        result.m_impl->m_name = datum_name;
    }

    const char* spheroid_name = gdal_spatial_ref.GetAttrValue("SPHEROID");
    if( spheroid_name )
    {
        result.m_impl->m_spheroid_name = spheroid_name;
    }

    const char* meridian_name = gdal_spatial_ref.GetAttrValue("PRIMEM");
    if( meridian_name )
    {
        result.m_impl->m_meridian_name = meridian_name;
    }

    OGRErr e1, e2;
    double semi_major = gdal_spatial_ref.GetSemiMajor( &e1 );
    double semi_minor = gdal_spatial_ref.GetSemiMinor( &e2 );
    if( e1 != OGRERR_FAILURE && e2 != OGRERR_FAILURE )
    {
        result.m_impl->set_semi_major_axis( semi_major );
        result.m_impl->set_semi_minor_axis( semi_minor );
    }
    result.m_impl->m_meridian_offset = gdal_spatial_ref.GetPrimeMeridian();

    char* proj4_str_tmp;
    gdal_spatial_ref.exportToProj4( &proj4_str_tmp );

    result.m_impl->m_proj_str = proj4_str_tmp;
    CPLFree( proj4_str_tmp );

    return outcome::ok<Datum>( std::move( result ) );
}

/************************************************/
/*          Convert from Proj String            */
/************************************************/
ImageResult<Datum> Datum::from_proj_string( const std::string& proj_str )
{
    OGRSpatialReference gdal_spatial_ref;
    if( gdal_spatial_ref.importFromProj4( proj_str.c_str() ) )
    {
        return outcome::fail( core::error::ErrorCode::PARSING_ERROR,
                              "Failed to parse: \"", proj_str, "\"." );
    }
    
    return from_ogr_spatial_ref( gdal_spatial_ref );
}

} // End of tmns::coordinate