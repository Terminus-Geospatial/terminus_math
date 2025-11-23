/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/*                                                                                    */
/*                           Copyright (c) 2024 Terminus LLC                          */
/*                                                                                    */
/*                                All Rights Reserved.                                */
/*                                                                                    */
/*          Use of this source code is governed by LICENSE in the repo root.          */
/*                                                                                    */
/***************************# INTELLECTUAL PROPERTY RIGHTS ****************************/
/**
 * @file    TEST_point.cpp
 * @author  Marvin Smith
 * @date    7/12/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/log/utility.hpp>
#include <terminus/math/Point.hpp>
#include <terminus/math/Point_Utilities.hpp>
#include <terminus/math/vector.hpp>

namespace tmx = tmns::math;

/****************************************/
/*          Test the Constructors       */
/****************************************/
TEST( Point, Constructors )
{
    const double EPS = 0.001;

    // Point2d tests
    tmns::math::Point2d pt1;
    ASSERT_NEAR( pt1.x(), 0, EPS );
    ASSERT_NEAR( pt1.y(), 0, EPS );

}

/****************************************************************/
/*      Test the Assignment Operator and Copy Constructors      */
/****************************************************************/
TEST( Point, Conversions )
{
    //////////////////////////////////////////////////////////
    //  TEST 1: Create a Binary Vector (from adding vectors)
    //////////////////////////////////////////////////////////
    tmx::VectorNd vec_01( 4, 1 );
    tmx::Vector4d vec_02( { 1, 2, 3, 4 } );

    auto vec_03 = vec_01 + vec_02;

    // Convert to a point using copy-constructor
    tmx::Point4d result_01( vec_03 );

    for( size_t i=0; i<4; i++ )
    {
        ASSERT_NEAR( result_01[i], i+2, 0.001 );
    }

    // Convert to a point using assignment operator
    tmx::Point4d result_02( vec_03 );

    for( size_t i=0; i<4; i++ )
    {
        ASSERT_NEAR( result_02[i], i+2, 0.001 );
    }
}

/************************************************/
/*          Test the Addition Operator          */
/************************************************/
TEST( Point, Addition_Operator )
{
    auto pt1 = tmns::math::ToPoint2<double>( 3, -5 );
    auto pt2 = tmns::math::ToPoint2<double>( 7, 8 );
    auto pt3 = pt1 + pt2;

    ASSERT_NEAR( pt3.x(), 10, 0.001 );
    ASSERT_NEAR( pt3.y(),  3, 0.001 );


    auto pt4 = tmns::math::ToPoint3<double>( 3, -5, 5);
    auto pt5 = tmns::math::ToPoint3<double>( 7, 8, -5 );
    auto pt6 = pt4 + pt5;

    ASSERT_NEAR( pt6.x(), 10, 0.001 );
    ASSERT_NEAR( pt6.y(),  3, 0.001 );
    ASSERT_NEAR( pt6.z(),  0, 0.001 );

    // Test a point with a vector
    auto vec7 = tmns::math::Vector3d( { -1.1, -2.2, -3.3 } );
    auto pt8  = pt4 + vec7;
    ASSERT_NEAR( pt8.x(), 1.9, 0.001 );
    ASSERT_NEAR( pt8.y(),-7.2, 0.001 );
    ASSERT_NEAR( pt8.z(), 1.7, 0.001 );

}

/***************************************************/
/*          Test the Subtraction Operator          */
/***************************************************/
TEST( Point, Subtraction_Operator )
{
    auto pt1 = tmns::math::ToPoint2<double>( 3, -5 );
    auto pt2 = tmns::math::ToPoint2<double>( 7, 8 );
    auto pt3 = pt1 - pt2;

    ASSERT_NEAR( pt3.x(), -4, 0.001 );
    ASSERT_NEAR( pt3.y(),-13, 0.001 );


    auto pt4 = tmns::math::ToPoint3<double>( 3, -5, 5);
    auto pt5 = tmns::math::ToPoint3<double>( 7, 8, -5 );
    auto pt6 = pt4 - pt5;

    ASSERT_NEAR( pt6.x(), -4, 0.001 );
    ASSERT_NEAR( pt6.y(),-13, 0.001 );
    ASSERT_NEAR( pt6.z(), 10, 0.001 );

}

/************************************/
/*      Test the Mag2 Function      */
/************************************/
TEST( Point, magnitude_sq )
{
    auto pt1 = tmns::math::ToPoint3<double>( -1, 2, -3 );

    ASSERT_NEAR( pt1.magnitude_sq(), 14, 0.01 );
}

/****************************************/
/*      Test the Distance Function      */
/****************************************/
TEST( Point, distance )
{
    auto pt1 = tmns::math::ToPoint3<double>( -1, -1, -1 );
    auto pt2 = tmns::math::ToPoint3<double>(  5,  5,  5 );
    auto method = tmns::math::DistanceType::L2;

    ASSERT_NEAR( tmns::math::Point3d::distance( pt1, pt2, method ), 10.392, 0.01 );
}

/***********************************************/
/*      Test the elementwise min Function      */
/***********************************************/
TEST( Point, elementwise_min )
{
    // Create a bunch of points
    auto pt1 = tmns::math::ToPoint3<double>(  0,  1,  2 );
    auto pt2 = tmns::math::ToPoint3<double>(  1,  0, -1 );
    auto pt3 = tmns::math::ToPoint3<double>( -2, -3,  4 );

    // Answer should be [0, 0, -1]
    auto pt_res1 = tmns::math::Point3d::elementwise_min( pt1, pt2 );
    ASSERT_NEAR( pt_res1[0], 0, 0.001 );
    ASSERT_NEAR( pt_res1[1], 0, 0.001 );
    ASSERT_NEAR( pt_res1[2],-1, 0.001 );

    // Answer should be [-2, -3, -1]
    auto pt_res2 = tmns::math::Point3d::elementwise_min( pt1, pt2, pt3 );
    ASSERT_NEAR( pt_res2[0],-2, 0.001 );
    ASSERT_NEAR( pt_res2[1],-3, 0.001 );
    ASSERT_NEAR( pt_res2[2],-1, 0.001 );

}