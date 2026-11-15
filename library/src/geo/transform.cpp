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
 * @file    transform.cpp
 * @author  Marvin Smith
 * @date    11/15/2026
*/

// Terminus Libraries
#include <terminus/geo/conversions.hpp>
#include <terminus/geo/transform.hpp>

namespace tmns::geo {

namespace {

/// Zone + hemisphere for a natively-modeled UTM CRS; 0 when not UTM.
std::int32_t utm_zone( const CRS& crs )
{
    if( !crs.is_projected() || crs.projection_method() != "transverse_mercator" )
    {
        return 0;
    }
    const auto& params = crs.projection_params();
    const auto  it     = params.find( "zone" );
    return it == params.end() ? 0 : static_cast<std::int32_t>( it->second );
}

/// Both CRSs carry the same modeled horizontal datum.
bool same_datum( const CRS& a, const CRS& b )
{
    return a.datum().has_value() && b.datum().has_value() &&
           *a.datum() == *b.datum();
}

/// Geographic (lon_deg, lat_deg, h) <-> ECEF (x, y, z).
Result<math::Point3d> geo_to_ecef( const math::Point3d& p, const Datum& datum )
{
    auto geo = coord::Geographic::from_degrees( p[1], p[0], p[2], datum );
    if( geo.has_error() )
    {
        return outcome::fail( geo.error() );
    }
    auto ecef = coord::to_ecef( geo.value() );
    if( ecef.has_error() )
    {
        return outcome::fail( ecef.error() );
    }
    const auto& v = ecef.value();
    return outcome::ok<math::Point3d>( math::Point3d( { v.x(), v.y(), v.z() } ) );
}

Result<math::Point3d> ecef_to_geo( const math::Point3d& p, const Datum& datum )
{
    auto ecef = coord::ECEF::create( p[0], p[1], p[2], datum );
    if( ecef.has_error() )
    {
        return outcome::fail( ecef.error() );
    }
    auto geo = coord::to_geographic( ecef.value() );
    if( geo.has_error() )
    {
        return outcome::fail( geo.error() );
    }
    const auto& v = geo.value();
    return outcome::ok<math::Point3d>(
        math::Point3d( { v.longitude_degrees(), v.latitude_degrees(), v.elevation_meters() } ) );
}

/// Geographic (lon_deg, lat_deg, h) <-> UTM (easting, northing, h).
Result<math::Point3d> geo_to_utm( const math::Point3d& p,
                                  const Datum&           datum,
                                  std::int32_t           zone,
                                  coord::Hemisphere      hemisphere )
{
    auto geo = coord::Geographic::from_degrees( p[1], p[0], p[2], datum );
    if( geo.has_error() )
    {
        return outcome::fail( geo.error() );
    }
    auto utm = coord::to_utm( geo.value(), zone, hemisphere );
    if( utm.has_error() )
    {
        return outcome::fail( utm.error() );
    }
    const auto& v = utm.value();
    return outcome::ok<math::Point3d>(
        math::Point3d( { v.easting_meters(), v.northing_meters(), v.elevation_meters() } ) );
}

Result<math::Point3d> utm_to_geo( const math::Point3d& p,
                                  const Datum&           datum,
                                  std::int32_t           zone,
                                  coord::Hemisphere      hemisphere )
{
    auto utm = coord::UTM::create( zone, hemisphere, p[0], p[1], p[2], datum );
    if( utm.has_error() )
    {
        return outcome::fail( utm.error() );
    }
    auto geo = coord::to_geographic( utm.value() );
    if( geo.has_error() )
    {
        return outcome::fail( geo.error() );
    }
    const auto& v = geo.value();
    return outcome::ok<math::Point3d>(
        math::Point3d( { v.longitude_degrees(), v.latitude_degrees(), v.elevation_meters() } ) );
}

/// Hemisphere encoded by a UTM CRS's false northing.
coord::Hemisphere utm_hemisphere( const CRS& crs )
{
    const auto& params = crs.projection_params();
    const auto  it     = params.find( "false_northing" );
    return ( it != params.end() && it->second != 0.0 )
        ? coord::Hemisphere::SOUTH
        : coord::Hemisphere::NORTH;
}

} // end of anonymous namespace


/****************************************/
/*              create                  */
/****************************************/
Result<Transform> Transform::create( const CRS& source, const CRS& target )
{
    Transform transform;
    transform.m_source = source;
    transform.m_target = target;

    // Identity — no work needed either direction.
    if( source == target )
    {
        transform.m_identity = true;
        return outcome::ok<Transform>( transform );
    }

    // Native closed-form paths require matching modeled datums.
    if( !same_datum( source, target ) )
    {
        return outcome::fail( error::Error_Code::NOT_IMPLEMENTED,
                              "No native transform path for this CRS pair; "
                              "inject a backend via create_with_backend" );
    }
    const Datum& datum = *source.datum();

    const std::int32_t src_zone = utm_zone( source );
    const std::int32_t dst_zone = utm_zone( target );

    // Geographic <-> geocentric (ECEF).
    if( source.is_geographic() && target.is_geocentric() )
    {
        transform.m_forward = [datum]( const math::Point3d& p ) { return geo_to_ecef( p, datum ); };
        transform.m_inverse = [datum]( const math::Point3d& p ) { return ecef_to_geo( p, datum ); };
        transform.m_elevation_policy = math::Elevation_Policy::PRODUCE;
        return outcome::ok<Transform>( transform );
    }
    if( source.is_geocentric() && target.is_geographic() )
    {
        transform.m_forward = [datum]( const math::Point3d& p ) { return ecef_to_geo( p, datum ); };
        transform.m_inverse = [datum]( const math::Point3d& p ) { return geo_to_ecef( p, datum ); };
        transform.m_elevation_policy = math::Elevation_Policy::PRODUCE;
        return outcome::ok<Transform>( transform );
    }

    // Geographic <-> projected (UTM); elevation passes through.
    if( source.is_geographic() && dst_zone != 0 )
    {
        transform.m_forward = [datum, dst_zone, target]( const math::Point3d& p )
        {
            return geo_to_utm( p, datum, dst_zone, utm_hemisphere( target ) );
        };
        transform.m_inverse = [datum, dst_zone, target]( const math::Point3d& p )
        {
            return utm_to_geo( p, datum, dst_zone, utm_hemisphere( target ) );
        };
        return outcome::ok<Transform>( transform );
    }
    if( src_zone != 0 && target.is_geographic() )
    {
        transform.m_forward = [datum, src_zone, source]( const math::Point3d& p )
        {
            return utm_to_geo( p, datum, src_zone, utm_hemisphere( source ) );
        };
        transform.m_inverse = [datum, src_zone, source]( const math::Point3d& p )
        {
            return geo_to_utm( p, datum, src_zone, utm_hemisphere( source ) );
        };
        return outcome::ok<Transform>( transform );
    }

    return outcome::fail( error::Error_Code::NOT_IMPLEMENTED,
                          "No native transform path for this CRS pair; "
                          "inject a backend via create_with_backend" );
}


/****************************************/
/*         create_with_backend          */
/****************************************/
Transform Transform::create_with_backend( const CRS&              source,
                                          const CRS&              target,
                                          Step                    forward_step,
                                          Step                    inverse_step,
                                          math::Elevation_Policy  policy )
{
    Transform transform;
    transform.m_source           = source;
    transform.m_target           = target;
    transform.m_forward          = std::move( forward_step );
    transform.m_inverse          = std::move( inverse_step );
    transform.m_elevation_policy = policy;
    return transform;
}


/****************************************/
/*              forward                 */
/****************************************/
Result<math::Point3d> Transform::forward( const math::Point3d& p ) const
{
    if( m_identity )
    {
        return outcome::ok<math::Point3d>( p );
    }
    if( !m_forward )
    {
        return outcome::fail( error::Error_Code::NOT_SUPPORTED,
                              "Transform has no forward step" );
    }
    return m_forward( p );
}


/****************************************/
/*              inverse                 */
/****************************************/
Result<math::Point3d> Transform::inverse( const math::Point3d& p ) const
{
    if( m_identity )
    {
        return outcome::ok<math::Point3d>( p );
    }
    if( !m_inverse )
    {
        return outcome::fail( error::Error_Code::NOT_SUPPORTED,
                              "Transform has no inverse step" );
    }
    return m_inverse( p );
}


/****************************************/
/*         forward (2D)                 */
/****************************************/
Result<math::Point2d> Transform::forward( const math::Point2d& p ) const
{
    auto result = forward( math::Point3d( { p[0], p[1], 0.0 } ) );
    if( result.has_error() )
    {
        return outcome::fail( result.error() );
    }
    const auto& v = result.value();
    return outcome::ok<math::Point2d>( math::Point2d( { v[0], v[1] } ) );
}


/****************************************/
/*         inverse (2D)                 */
/****************************************/
Result<math::Point2d> Transform::inverse( const math::Point2d& p ) const
{
    auto result = inverse( math::Point3d( { p[0], p[1], 0.0 } ) );
    if( result.has_error() )
    {
        return outcome::fail( result.error() );
    }
    const auto& v = result.value();
    return outcome::ok<math::Point2d>( math::Point2d( { v[0], v[1] } ) );
}


/****************************************/
/*            has_forward               */
/****************************************/
bool Transform::has_forward() const
{
    return m_identity || static_cast<bool>( m_forward );
}


/****************************************/
/*            has_inverse               */
/****************************************/
bool Transform::has_inverse() const
{
    return m_identity || static_cast<bool>( m_inverse );
}


/****************************************/
/*              source                  */
/****************************************/
const CRS& Transform::source() const
{
    return m_source;
}


/****************************************/
/*              target                  */
/****************************************/
const CRS& Transform::target() const
{
    return m_target;
}


/****************************************/
/*         elevation_policy             */
/****************************************/
math::Elevation_Policy Transform::elevation_policy() const
{
    return m_elevation_policy;
}


/****************************************/
/*        set_elevation_policy          */
/****************************************/
void Transform::set_elevation_policy( math::Elevation_Policy policy )
{
    m_elevation_policy = policy;
}


} // end of tmns::geo namespace
