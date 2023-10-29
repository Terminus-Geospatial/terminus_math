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

/*****************************************************************************/
/*      Convert a 3D Coordinate from Geodetic to Cartesian Coordinates       */
/*****************************************************************************/
math::Vector3d Datum::geodetic_to_cartesian( const math::Vector3d& llh ) const
{
    double a  = m_impl->m_semi_major_axis;
    double b  = m_impl->m_semi_minor_axis;
    double a2 = a * a;
    double b2 = b * b;
    double e2 = (a2 - b2) / a2;

    double lat = llh.y();
    if ( lat < -90 ) lat = -90;
    if ( lat >  90 ) lat = 90;

    double rlon = ( llh.x() + m_impl->m_meridian_offset ) * ( M_PI / 180 );
    double rlat = lat * ( M_PI / 180 );
    double slat = std::sin( rlat );
    double clat = std::cos( rlat );
    double slon = std::sin( rlon );
    double clon = std::cos( rlon );
    double radius = a / std::sqrt( 1.0 - e2 * slat * slat );

    return math::Vector3d( { ( radius + llh.z() ) * clat * clon,
                             ( radius + llh.z() ) * clat * slon,
                             ( radius * ( 1 - e2 ) + llh.z() ) * slat } );
}   

/**
 * This algorithm is a non-iterative algorithm from "An analytical method to transform geocentric into geodetic
 * coordinates" by Hugues Vermeille, Journal of Geodesy 2011.
 * 
 * This is an improvement over the 1988/Proj4's implementation as it's a smidgen faster and it still works near the
 * center of the datum.
 */
math::Vector3d Datum::cartesian_to_geodetic( const math::Vector3d& xyz ) const
{
    const double a2 = m_impl->m_semi_major_axis * m_impl->m_semi_major_axis;
    const double b2 = m_impl->m_semi_minor_axis * m_impl->m_semi_minor_axis;
    const double e2 = 1 - b2 / a2;
    const double e4 = e2 * e2;

    double xy_dist = std::sqrt( xyz[0] * xyz[0] + xyz[1] * xyz[1] );
    double p  = ( xyz[0] * xyz[0] + xyz[1] * xyz[1] ) / a2;
    double q  = ( 1 - e2 ) * xyz[2] * xyz[2] / a2;
    double r  = ( p + q - e4 ) / 6.0;
    double r3 = r * r * r;

    math::Vector3d llh;

    double evolute = 8 * r3 + e4 * p * q;
    double u = std::numeric_limits<double>::quiet_NaN();
    if ( evolute > 0 )
    {
        // outside the evolute
        double right_inside_pow = std::sqrt(e4 * p * q);
        double sqrt_evolute = std::sqrt( evolute );
        u = r + 0.5 * std::pow( sqrt_evolute + right_inside_pow, 2.0/3.0 )
          + 0.5 * std::pow( sqrt_evolute - right_inside_pow, 2.0/3.0 );
    }
    else if( std::fabs( xyz[2] ) < std::numeric_limits<double>::epsilon() )
    {
        // On the equator plane
        llh[1] = 0;
        llh[2] = xyz.magnitude() - m_impl->m_semi_major_axis;
    }
    else if( evolute < 0 and std::fabs(q) > std::numeric_limits<double>::epsilon() )
    {
        // On or inside the evolute
        double atan_result = std::atan2( sqrt( e4 * p * q ), std::sqrt( -evolute ) + std::sqrt(-8 * r3) );
        u = -4 * r * std::sin( 2.0 / 3.0 * atan_result )
                   * std::cos( M_PI / 6.0 + 2.0 / 3.0 * atan_result );
    }
    else if( std::fabs(q) < std::numeric_limits<double>::epsilon() and p <= e4 )
    {
        // In the singular disc
        llh[2] = -m_impl->m_semi_major_axis * std::sqrt(1 - e2) * std::sqrt(e2 - p) / std::sqrt(e2);
        llh[1] = 2 * std::atan2( std::sqrt(e4 - p), std::sqrt(e2*(e2 - p)) + std::sqrt(1-e2) * std::sqrt(p) );
    }
    else
    {
        // Near the cusps of the evolute
        double inside_pow = sqrt(evolute) + sqrt(e4 * p * q);
        u = r + 0.5   * std::pow( inside_pow,  2.0/3.0 )
              + 2*r*r * std::pow( inside_pow, -2.0/3.0 );
    }

    if( !std::isnan(u) )
    {
        double v   = std::sqrt( u * u + e4 * q );
        double u_v = u + v;
        double w   = e2 * ( u_v - q ) / ( 2 * v );
        double k   = u_v / ( w + std::sqrt( w * w + u_v ) );
        double D   = k * xy_dist / ( k + e2 );
        double dist_2 = D * D + xyz[2] * xyz[2];
        llh[2] = ( k + e2 - 1 ) * std::sqrt( dist_2 ) / k;
        llh[1] = 2 * std::atan2( xyz[2], std::sqrt( dist_2 ) + D );
    }

    if ( xy_dist + xyz[0] > ( std::sqrt(2) - 1 ) * xyz[1] )
    {
        // Longitude is between -135 and 135
        llh[0] = 360.0 * std::atan2( xyz[1], xy_dist + xyz[0] ) / M_PI;
    }
    else if( xy_dist + xyz[1] < ( std::sqrt(2) + 1 ) * xyz[0] )
    {
        // Longitude is between -225 and 45
        llh[0] = - 90.0 + 360.0 * std::atan2( xyz[0], xy_dist - xyz[1] ) / M_PI;
    }
    else
    {
        // Longitude is between -45 and 225
        llh[0] = 90.0 - 360.0 * std::atan2( xyz[0], xy_dist + xyz[1] ) / M_PI;
    }
    llh[0] -= m_impl->m_meridian_offset;
    llh[1] *= 180.0 / M_PI;

    return llh;
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