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
 * @file    TEST_transform.cpp
 * @author  Marvin Smith
 * @date    11/15/2026
 *
 * @brief   Dispatch tests for geo::Transform — identity, native
 *          closed-form pairs, backend injection, and failure modes.
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/geo/crs.hpp>
#include <terminus/geo/transform.hpp>

using namespace tmns::geo;
using tmns::math::Point2d;
using tmns::math::Point3d;
namespace outcome = tmns::outcome;

namespace {

constexpr double DEG_TOL = 1e-9;
constexpr double M_TOL   = 1e-3;

} // end of anonymous namespace


/****************************************/
/*             Identity                 */
/****************************************/
TEST( Transform_Test, identical_crs_is_identity )
{
    auto xform = Transform::create( CRS::wgs84(), CRS::wgs84() );
    ASSERT_FALSE( xform.has_error() );

    const Point3d in( { -105.0, 40.0, 1609.0 } );
    auto out = xform.value().forward( in );
    ASSERT_FALSE( out.has_error() );
    EXPECT_EQ( out.value()[0], in[0] );
    EXPECT_EQ( out.value()[1], in[1] );
    EXPECT_EQ( out.value()[2], in[2] );
}


/****************************************/
/*       geographic <-> ECEF            */
/****************************************/
TEST( Transform_Test, wgs84_to_ecef_round_trip )
{
    auto xform = Transform::create( CRS::wgs84(), CRS::ecef() );
    ASSERT_FALSE( xform.has_error() );
    EXPECT_TRUE( xform.value().has_forward() );
    EXPECT_TRUE( xform.value().has_inverse() );

    const Point3d denver( { -104.9903, 39.7392, 1609.0 } );
    auto ecef = xform.value().forward( denver );
    ASSERT_FALSE( ecef.has_error() );

    // Round-trip back.
    auto back = xform.value().inverse( ecef.value() );
    ASSERT_FALSE( back.has_error() );
    EXPECT_NEAR( back.value()[0], denver[0], DEG_TOL );
    EXPECT_NEAR( back.value()[1], denver[1], DEG_TOL );
    EXPECT_NEAR( back.value()[2], denver[2], M_TOL );
}


/****************************************/
/*       geographic <-> UTM             */
/****************************************/
TEST( Transform_Test, wgs84_to_utm13n_round_trip )
{
    auto utm13n = CRS::utm( 13, true );
    ASSERT_FALSE( utm13n.has_error() );

    auto xform = Transform::create( CRS::wgs84(), utm13n.value() );
    ASSERT_FALSE( xform.has_error() );

    const Point3d pt( { -105.0, 39.0, 1500.0 } );
    auto projected = xform.value().forward( pt );
    ASSERT_FALSE( projected.has_error() );

    // Sanity: zone-13 interior points have plausible eastings.
    EXPECT_GT( projected.value()[0], 200000.0 );
    EXPECT_LT( projected.value()[0], 800000.0 );
    EXPECT_NEAR( projected.value()[2], pt[2], 1e-9 );

    auto back = xform.value().inverse( projected.value() );
    ASSERT_FALSE( back.has_error() );
    EXPECT_NEAR( back.value()[0], pt[0], 1e-7 );
    EXPECT_NEAR( back.value()[1], pt[1], 1e-7 );
}


/****************************************/
/*        2D convenience API            */
/****************************************/
TEST( Transform_Test, point2d_uses_3d_path )
{
    // 2D entry points are only meaningful for horizontal transforms —
    // z can't round-trip through a 3D space like ECEF.
    auto utm13n = CRS::utm( 13, true );
    ASSERT_FALSE( utm13n.has_error() );
    auto xform = Transform::create( CRS::wgs84(), utm13n.value() );
    ASSERT_FALSE( xform.has_error() );

    const Point2d in( { -105.0, 39.0 } );
    auto out = xform.value().forward( in );
    ASSERT_FALSE( out.has_error() );

    auto back = xform.value().inverse( out.value() );
    ASSERT_FALSE( back.has_error() );
    EXPECT_NEAR( back.value()[0], in[0], 1e-7 );
    EXPECT_NEAR( back.value()[1], in[1], 1e-7 );
}


/****************************************/
/*         Unsupported pairs            */
/****************************************/
TEST( Transform_Test, unknown_pair_fails )
{
    // Two engineering frames have no modeled relation.
    auto a = CRS::engineering( "frame_a", Axis_Set::ecef_meters() );
    auto b = CRS::engineering( "frame_b", Axis_Set::ecef_meters() );
    auto xform = Transform::create( a, b );
    EXPECT_TRUE( xform.has_error() );
}


/****************************************/
/*         Backend injection            */
/****************************************/
TEST( Transform_Test, injected_backend_is_used )
{
    // A trivial "backend" that swaps axes — proves the steps are invoked.
    auto fwd = []( const Point3d& p ) -> Result<Point3d>
    {
        return outcome::ok<Point3d>( Point3d( { p[1], p[0], p[2] } ) );
    };
    auto inv = []( const Point3d& p ) -> Result<Point3d>
    {
        return outcome::ok<Point3d>( Point3d( { p[1], p[0], p[2] } ) );
    };

    auto xform = Transform::create_with_backend(
        CRS::wgs84(), CRS::ecef(), fwd, inv );

    auto out = xform.forward( Point3d( { 1.0, 2.0, 3.0 } ) );
    ASSERT_FALSE( out.has_error() );
    EXPECT_EQ( out.value()[0], 2.0 );
    EXPECT_EQ( out.value()[1], 1.0 );
}


/****************************************/
/*        Missing direction             */
/****************************************/
TEST( Transform_Test, missing_inverse_step_fails )
{
    auto fwd = []( const Point3d& p ) -> Result<Point3d>
    {
        return outcome::ok<Point3d>( p );
    };
    auto xform = Transform::create_with_backend(
        CRS::wgs84(), CRS::ecef(), fwd, {} );

    EXPECT_TRUE( xform.has_forward() );
    EXPECT_FALSE( xform.has_inverse() );
    auto out = xform.inverse( Point3d( { 1.0, 2.0, 3.0 } ) );
    EXPECT_TRUE( out.has_error() );
}


/****************************************/
/*           CRS factories              */
/****************************************/
TEST( CRS_Test, factories_produce_modeled_crs )
{
    auto wgs = CRS::wgs84();
    EXPECT_TRUE( wgs.is_geographic() );
    EXPECT_EQ( wgs.code(), 4326 );
    EXPECT_EQ( wgs.axes().size(), 2u );
    EXPECT_TRUE( wgs.axes().is_traditional_gis_order() );

    auto ecef = CRS::ecef();
    EXPECT_TRUE( ecef.is_geocentric() );
    EXPECT_EQ( ecef.code(), 4978 );
    EXPECT_EQ( ecef.axes().size(), 3u );

    auto utm13 = CRS::utm( 13, true );
    ASSERT_FALSE( utm13.has_error() );
    EXPECT_TRUE( utm13.value().is_projected() );
    EXPECT_EQ( utm13.value().code(), 32613 );
    EXPECT_EQ( utm13.value().projection_method(), "transverse_mercator" );
    EXPECT_EQ( utm13.value().projection_params().at( "zone" ), 13.0 );

    auto bad = CRS::utm( 99, true );
    EXPECT_TRUE( bad.has_error() );
}


/****************************************/
/*          CRS from_epsg               */
/****************************************/
TEST( CRS_Test, from_epsg_native_codes )
{
    auto wgs = CRS::from_epsg( 4326 );
    ASSERT_FALSE( wgs.has_error() );
    EXPECT_TRUE( wgs.value().is_geographic() );

    auto utm = CRS::from_epsg( 32613 );
    ASSERT_FALSE( utm.has_error() );
    EXPECT_TRUE( utm.value().is_projected() );

    auto south = CRS::from_epsg( 32713 );
    ASSERT_FALSE( south.has_error() );
    EXPECT_EQ( south.value().projection_params().at( "false_northing" ), 10000000.0 );

    auto unknown = CRS::from_epsg( 2193 );  // NZTM — not natively modeled
    EXPECT_TRUE( unknown.has_error() );
}


/****************************************/
/*           Axis_Set                   */
/****************************************/
TEST( Axis_Set_Test, factories )
{
    auto ll = Axis_Set::lon_lat_degrees();
    EXPECT_TRUE( ll.is_traditional_gis_order() );

    auto lt = Axis_Set::lat_lon_degrees();
    EXPECT_FALSE( lt.is_traditional_gis_order() );

    auto en = Axis_Set::easting_northing_meters();
    EXPECT_TRUE( en.is_traditional_gis_order() );
    EXPECT_EQ( en[0].unit, "metre" );
}
