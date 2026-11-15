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
 * @file    TEST_affine_transform.cpp
 * @author  Marvin Smith
 * @date    11/15/2026
 *
 * @brief   Affine_Transform forward/inverse and geotransform mapping.
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/math/affine_transform.hpp>
#include <terminus/math/composite_transform.hpp>

using namespace tmns::math;
namespace outcome = tmns::outcome;

namespace {

constexpr double TOL = 1e-12;

} // end of anonymous namespace


/****************************************/
/*            Identity                  */
/****************************************/
TEST( Affine_Transform_Test, identity_is_noop )
{
    const auto id = Affine_Transform_2D::identity();
    const Point2d p( { 3.5, -7.25 } );
    const auto out = id.forward( p );
    EXPECT_NEAR( out[0], p[0], TOL );
    EXPECT_NEAR( out[1], p[1], TOL );
}


/****************************************/
/*         from_geotransform            */
/****************************************/
TEST( Affine_Transform_Test, geotransform_maps_pixels )
{
    // Landsat-style geotransform: origin (489885, 4426815), 30 m pixels,
    // north-up (negative y pixel size).
    const double gt[6] = { 489885.0, 30.0, 0.0, 4426815.0, 0.0, -30.0 };
    const auto affine = Affine_Transform_2D::from_geotransform( gt );

    // Pixel (0,0) -> origin.
    const auto origin = affine.forward( Point2d( { 0.0, 0.0 } ) );
    EXPECT_NEAR( origin[0], gt[0], TOL );
    EXPECT_NEAR( origin[1], gt[3], TOL );

    // Pixel (1,1) -> origin + (30, -30).
    const auto p11 = affine.forward( Point2d( { 1.0, 1.0 } ) );
    EXPECT_NEAR( p11[0], gt[0] + 30.0, TOL );
    EXPECT_NEAR( p11[1], gt[3] - 30.0, TOL );

    // Inverse recovers the pixel.
    const auto back = affine.inverse( p11 );
    ASSERT_FALSE( back.has_error() );
    EXPECT_NEAR( back.value()[0], 1.0, 1e-9 );
    EXPECT_NEAR( back.value()[1], 1.0, 1e-9 );
}


/****************************************/
/*         Forward/inverse              */
/****************************************/
TEST( Affine_Transform_Test, rotated_inverse_round_trip )
{
    // Rotation + scale + translation.
    const double c = std::cos( 0.3 ), s = std::sin( 0.3 );
    const Affine_Transform_2D::MatrixT m( {
        c * 2.0, -s * 2.0, 100.0,
        s * 2.0,  c * 2.0, -50.0,
        0.0,      0.0,      1.0
    } );
    const auto affine = Affine_Transform_2D::from_matrix( m );

    const Point2d p( { 17.25, -4.5 } );
    const auto fwd = affine.forward( p );
    const auto inv = affine.inverse( fwd );
    ASSERT_FALSE( inv.has_error() );
    EXPECT_NEAR( inv.value()[0], p[0], 1e-9 );
    EXPECT_NEAR( inv.value()[1], p[1], 1e-9 );
}


/****************************************/
/*           Singular matrix            */
/****************************************/
TEST( Affine_Transform_Test, singular_inverse_fails )
{
    const Affine_Transform_2D::MatrixT m( {
        1.0, 2.0, 0.0,
        2.0, 4.0, 0.0,   // row 1 = 2 * row 0 -> singular
        0.0, 0.0, 1.0
    } );
    const auto affine = Affine_Transform_2D::from_matrix( m );
    const auto inv = affine.inverse( Point2d( { 1.0, 1.0 } ) );
    EXPECT_TRUE( inv.has_error() );
}


/****************************************/
/*              3D case                 */
/****************************************/
TEST( Affine_Transform_Test, three_dimensional )
{
    auto affine = Affine_Transform_3D::identity();
    const Point3d p( { 1.0, 2.0, 3.0 } );
    const auto out = affine.forward( p );
    EXPECT_NEAR( out[0], 1.0, TOL );
    EXPECT_NEAR( out[1], 2.0, TOL );
    EXPECT_NEAR( out[2], 3.0, TOL );
}


/****************************************/
/*       Composite_Transform            */
/****************************************/
TEST( Composite_Transform_Test, ordering )
{
    // Chain: x -> +10 -> *2.  forward = (x+10)*2; inverse = x/2-10.
    Composite_Transform<Point2d> chain;
    chain.append( {
        []( const Point2d& p ) { return outcome::ok<Point2d>( Point2d( { p[0] + 10.0, p[1] } ) ); },
        []( const Point2d& p ) { return outcome::ok<Point2d>( Point2d( { p[0] - 10.0, p[1] } ) ); }
    } );
    chain.append( {
        []( const Point2d& p ) { return outcome::ok<Point2d>( Point2d( { p[0] * 2.0, p[1] } ) ); },
        []( const Point2d& p ) { return outcome::ok<Point2d>( Point2d( { p[0] / 2.0, p[1] } ) ); }
    } );

    const Point2d in( { 5.0, 1.0 } );
    auto fwd = chain.forward( in );
    ASSERT_FALSE( fwd.has_error() );
    EXPECT_NEAR( fwd.value()[0], 30.0, TOL );  // (5+10)*2

    auto back = chain.inverse( fwd.value() );
    ASSERT_FALSE( back.has_error() );
    EXPECT_NEAR( back.value()[0], in[0], TOL );
}


/****************************************/
/*   Composite missing direction        */
/****************************************/
TEST( Composite_Transform_Test, missing_step_direction_fails )
{
    Composite_Transform<Point2d> chain;
    chain.append( {
        []( const Point2d& p ) { return outcome::ok<Point2d>( p ); },
        {}  // no inverse
    } );

    EXPECT_FALSE( chain.forward( Point2d( { 1.0, 0.0 } ) ).has_error() );
    EXPECT_TRUE( chain.inverse( Point2d( { 1.0, 0.0 } ) ).has_error() );
}
