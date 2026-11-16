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
 * @file    TEST_clip_segment.cpp
 * @author  Marvin Smith
 * @date    11/15/2026
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/math/clip_segment.hpp>

namespace {

const tmns::math::Rect2d CLIP_RECT( 0.0, 0.0, 100.0, 100.0 );

void expect_segment( const tmns::math::Point2d& p1,
                     const tmns::math::Point2d& p2,
                     double x0, double y0, double x1, double y1 )
{
    const auto clipped = tmns::math::clip_segment_to_rect( p1, p2, CLIP_RECT );
    ASSERT_TRUE( clipped.has_value() );
    EXPECT_NEAR( clipped->first.x(),  x0, 1e-9 );
    EXPECT_NEAR( clipped->first.y(),  y0, 1e-9 );
    EXPECT_NEAR( clipped->second.x(), x1, 1e-9 );
    EXPECT_NEAR( clipped->second.y(), y1, 1e-9 );
}

void expect_rejected( const tmns::math::Point2d& p1,
                      const tmns::math::Point2d& p2 )
{
    EXPECT_FALSE( tmns::math::clip_segment_to_rect( p1, p2, CLIP_RECT ).has_value() );
}

} // end of anonymous namespace

/************************************************/
/*          Segment fully inside rect           */
/************************************************/
TEST( Clip_Segment, Inside )
{
    expect_segment( tmns::math::Point2d( { 10.0, 10.0 } ),
                    tmns::math::Point2d( { 90.0, 90.0 } ),
                    10.0, 10.0, 90.0, 90.0 );
}

/************************************************/
/*          Segment fully outside rect          */
/************************************************/
TEST( Clip_Segment, Outside )
{
    // Same-side rejections in each direction
    expect_rejected( tmns::math::Point2d( { -10.0, 10.0 } ),
                     tmns::math::Point2d( { -20.0, 90.0 } ) );
    expect_rejected( tmns::math::Point2d( { 110.0, 10.0 } ),
                     tmns::math::Point2d( { 120.0, 90.0 } ) );
    expect_rejected( tmns::math::Point2d( { 10.0, -10.0 } ),
                     tmns::math::Point2d( { 90.0, -20.0 } ) );
    expect_rejected( tmns::math::Point2d( { 10.0, 110.0 } ),
                     tmns::math::Point2d( { 90.0, 120.0 } ) );
}

/************************************************/
/*       Segments crossing the rect edges       */
/************************************************/
TEST( Clip_Segment, Crossing )
{
    // Horizontal through the rect
    expect_segment( tmns::math::Point2d( { -10.0, 50.0 } ),
                    tmns::math::Point2d( { 110.0, 50.0 } ),
                    0.0, 50.0, 100.0, 50.0 );

    // Vertical through the rect
    expect_segment( tmns::math::Point2d( { 50.0, -10.0 } ),
                    tmns::math::Point2d( { 50.0, 110.0 } ),
                    50.0, 0.0, 50.0, 100.0 );

    // Diagonal entering left, exiting top:  y = x + 75
    expect_segment( tmns::math::Point2d( { -50.0, 25.0 } ),
                    tmns::math::Point2d( { 75.0, 150.0 } ),
                    0.0, 75.0, 25.0, 100.0 );

    // One endpoint inside, other far outside
    expect_segment( tmns::math::Point2d( { 50.0, 50.0 } ),
                    tmns::math::Point2d( { 150.0, 50.0 } ),
                    50.0, 50.0, 100.0, 50.0 );

    // Both endpoints outside on opposite corners (diagonal through rect)
    expect_segment( tmns::math::Point2d( { -50.0, -50.0 } ),
                    tmns::math::Point2d( { 150.0, 150.0 } ),
                    0.0, 0.0, 100.0, 100.0 );
}

/************************************************/
/*   Both endpoints outside, same side (corner) */
/************************************************/
TEST( Clip_Segment, Corner_Bypass )
{
    // Diagonal outside the bottom-left corner; both endpoints share
    // LEFT|BOTTOM out-codes so no edge intersection is attempted
    expect_rejected( tmns::math::Point2d( { -50.0, -10.0 } ),
                     tmns::math::Point2d( { -10.0, -50.0 } ) );
}

/************************************************/
/*          Degenerate / boundary cases         */
/************************************************/
TEST( Clip_Segment, Degenerate )
{
    // Single point inside stays inside
    expect_segment( tmns::math::Point2d( { 50.0, 50.0 } ),
                    tmns::math::Point2d( { 50.0, 50.0 } ),
                    50.0, 50.0, 50.0, 50.0 );

    // Single point outside is rejected
    expect_rejected( tmns::math::Point2d( { 200.0, 50.0 } ),
                     tmns::math::Point2d( { 200.0, 50.0 } ) );

    // Endpoint exactly on the boundary is kept
    expect_segment( tmns::math::Point2d( { 0.0, 50.0 } ),
                    tmns::math::Point2d( { 50.0, 50.0 } ),
                    0.0, 50.0, 50.0, 50.0 );
}
