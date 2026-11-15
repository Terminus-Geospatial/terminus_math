/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/*                                                                                    */
/*                           Copyright (c) 2026 Terminus LLC                          */
/*                                                                                    */
/*                                All Rights Reserved.                                */
/*                                                                                    */
/*          Use of this source code is governed by LICENSE in the repo root.          */
/*                                                                                    */
/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/**
 * @file    TEST_bounds.cpp
 * @author  Marvin Smith
 * @date    11/15/2026
 *
 * @brief   Tests for geo::raster_bounds, geo::transform_bounds, and
 *          geo::calculate_default_georeference.
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/geo/bounds.hpp>
#include <terminus/geo/transform.hpp>

using namespace tmns::geo;

namespace {

/**
 * @brief Build a WGS-84 north-up georeference.
 *
 * gt maps pixel (0,0) center to (origin_lon, origin_lat) with
 * `res` degrees-per-pixel on both axes.
 */
Georeference_2D make_wgs84_georef( double origin_lon,
                                 double origin_lat,
                                 double res )
{
    auto crs = CRS::from_epsg( 4326 );
    EXPECT_FALSE( crs.has_error() ) << crs.error().details();
    const double gt[6] = { origin_lon, res, 0.0, origin_lat, 0.0, -res };
    return Georeference_2D::from_geotransform( gt, crs.value() );
}

} // end of anonymous namespace

/**
 * raster_bounds covers the pixel edges: -0.5 .. (size - 0.5) pushed
 * through the affine.
 */
TEST( Raster_Bounds, affine_corners )
{
    auto georef = make_wgs84_georef( -10.0, 40.0, 0.01 );

    auto bounds = raster_bounds( georef, tmns::math::Size2i( { 100, 50 } ) );
    ASSERT_FALSE( bounds.has_error() ) << bounds.error().details();
    EXPECT_DOUBLE_EQ( bounds.value().min().x(), -10.005 );
    EXPECT_DOUBLE_EQ( bounds.value().max().x(),  -9.005 );
    EXPECT_DOUBLE_EQ( bounds.value().min().y(),  39.505 );
    EXPECT_DOUBLE_EQ( bounds.value().max().y(),  40.005 );
}

/**
 * Identity transform: transformed extent matches the native bounds.
 */
TEST( Transform_Bounds, identity_preserves_extent )
{
    auto georef = make_wgs84_georef( -10.0, 40.0, 0.01 );
    tmns::math::Size2i src_size( { 100, 50 } );

    auto xform = Transform::create( georef.crs(), georef.crs() );
    ASSERT_FALSE( xform.has_error() );

    auto identity = [&xform]( const tmns::math::Point2d& p )
    {
        return xform.value().forward( p );
    };

    auto bounds = transform_bounds( georef, src_size, identity );
    ASSERT_FALSE( bounds.has_error() );
    EXPECT_DOUBLE_EQ( bounds.value().min().x(), -10.005 );
    EXPECT_DOUBLE_EQ( bounds.value().max().x(),  -9.005 );
}

/**
 * transform_edge_points returns the densified perimeter ring: 4*N
 * samples for N per edge, in dst-CRS coordinates.
 */
TEST( Transform_Edge_Points, ring_size_and_extent )
{
    auto georef = make_wgs84_georef( -10.0, 40.0, 0.01 );
    tmns::math::Size2i src_size( { 100, 50 } );

    auto xform = Transform::create( georef.crs(), georef.crs() );
    ASSERT_FALSE( xform.has_error() );

    auto identity = [&xform]( const tmns::math::Point2d& p )
    {
        return xform.value().forward( p );
    };

    auto ring = transform_edge_points( georef, src_size, identity, 5 );
    ASSERT_FALSE( ring.has_error() );
    EXPECT_EQ( ring.value().size(), 20u );

    // Every returned point must sit inside the source's native extent
    auto bounds = raster_bounds( georef, src_size );
    ASSERT_FALSE( bounds.has_error() );
    for( const auto& pt : ring.value() )
    {
        EXPECT_GE( pt.x(), bounds.value().min().x() );
        EXPECT_LE( pt.x(), bounds.value().max().x() );
        EXPECT_GE( pt.y(), bounds.value().min().y() );
        EXPECT_LE( pt.y(), bounds.value().max().y() );
    }
}

/**
 * calculate_default_georeference with an identity transform: output
 * extent and size mirror the source.
 */
TEST( Calculate_Default_Georeference, identity_preserves_extent )
{
    auto georef = make_wgs84_georef( -10.0, 40.0, 0.01 );
    tmns::math::Size2i src_size( { 100, 50 } );

    auto xform = Transform::create( georef.crs(), georef.crs() );
    ASSERT_FALSE( xform.has_error() );

    auto identity = [&xform]( const tmns::math::Point2d& p )
    {
        return xform.value().forward( p );
    };

    auto output = calculate_default_georeference( georef, src_size,
                                                  georef.crs(), identity );
    ASSERT_FALSE( output.has_error() ) << output.error().details();
    EXPECT_EQ( output.value().size.width(),  100 );
    EXPECT_EQ( output.value().size.height(), 50 );
}

/**
 * Native CRS path: 4326 -> UTM 31N.  A 1-degree source box spanning
 * lon 3-4 and crossing the equator produces a plausible raster.
 * Southern-latitude samples must yield negative northing (the target
 * CRS's declared hemisphere), not a 10M false-northing jump.
 */
TEST( Calculate_Default_Georeference, wgs84_to_utm )
{
    // Source: 1x1 degree box, lon 3.0->4.0, lat +0.5->-0.5
    auto src_georef = make_wgs84_georef( 3.0, 0.5, 0.001 );
    tmns::math::Size2i src_size( { 1000, 1000 } );

    auto utm_crs = CRS::from_epsg( 32631 );
    ASSERT_FALSE( utm_crs.has_error() );

    auto xform = Transform::create( src_georef.crs(), utm_crs.value() );
    ASSERT_FALSE( xform.has_error() ) << xform.error().details();

    auto src_to_dst = [&xform]( const tmns::math::Point2d& p )
    {
        return xform.value().forward( p );
    };

    auto output = calculate_default_georeference( src_georef, src_size,
                                                  utm_crs.value(),
                                                  src_to_dst );
    ASSERT_FALSE( output.has_error() ) << output.error().details();

    // Center of the dst extent: easting ~555 km (lon 3.5 in zone 31N)
    // and northing ~0 (symmetric across the equator).
    auto bounds = transform_bounds( src_georef, src_size, src_to_dst );
    ASSERT_FALSE( bounds.has_error() );
    const double center_e = ( bounds.value().min().x() +
                              bounds.value().max().x() ) / 2.0;
    const double center_n = ( bounds.value().min().y() +
                              bounds.value().max().y() ) / 2.0;
    EXPECT_NEAR( center_e, 555585.0, 5000.0 );
    EXPECT_NEAR( center_n,      0.0, 5000.0 );
    EXPECT_LT( bounds.value().min().y(), 0.0 );   // south -> negative N
    EXPECT_GT( bounds.value().max().y(), 0.0 );

    // Roughly square output for a square degree input near the equator
    EXPECT_GT( output.value().size.width(),  500 );
    EXPECT_GT( output.value().size.height(), 500 );
}
