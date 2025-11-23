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
 * @file    TEST_vectorn.cpp
 * @author  Marvin Smith
 * @date    7/30/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/math/vector/VectorN.hpp>

/***************************************************/
/*      Test Constructors, Getters, and Setters    */
/***************************************************/
TEST( VectorN, simple_operations )
{
    // Create a vector
    tmns::math::VectorN<double> vec01( { 0, 1, 2, 3 } );

    ASSERT_EQ( vec01.size(), 4 );

    // Set a specific value
    ASSERT_NEAR( vec01[2], 2, 0.001 );
    vec01[2] = -1;
    ASSERT_NEAR( vec01[2], -1, 0.001 );

    ASSERT_NEAR( vec01.x(), 0, 0.001 );
    ASSERT_NEAR( vec01.y(), 1, 0.001 );
    ASSERT_NEAR( vec01.z(),-1, 0.001 );

    // Test 2nd constructor
    tmns::math::Vector4_<double> vec_temp( { 0, 1, 2, 3 } );
    tmns::math::VectorN<double> vec02( vec_temp );

    ASSERT_NEAR( vec02.x(), 0, 0.001 );
    ASSERT_NEAR( vec02.y(), 1, 0.001 );
    ASSERT_NEAR( vec02.z(), 2, 0.001 );
    ASSERT_NEAR( vec02[3],  3, 0.001 );

}

/************************************************/
/*      Test Various Vector Constructors        */
/************************************************/
TEST( VectorN, Constructors )
{
    // Create an array
    std::array<double,3> arr01 { 1, 2, 3 };

    // Create Vector
    tmns::math::VectorNd vec01( arr01 );
    ASSERT_NEAR( vec01[0], 1, 0.001 );
    ASSERT_NEAR( vec01[1], 2, 0.001 );
    ASSERT_NEAR( vec01[2], 3, 0.001 );

    // Create another vector with raw pointer constructor
    tmns::math::VectorNd vec02( arr01.data(), arr01.size() );
    ASSERT_NEAR( vec02[0], 1, 0.001 );
    ASSERT_NEAR( vec02[1], 2, 0.001 );
    ASSERT_NEAR( vec02[2], 3, 0.001 );
}