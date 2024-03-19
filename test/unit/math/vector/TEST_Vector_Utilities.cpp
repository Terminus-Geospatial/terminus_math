/**
 * @file    TEST_Vector_Utilities.cpp
 * @author  Marvin Smith
 * @date    7/23/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/log/utility.hpp>
#include <terminus/math/vector/Vector_Utilities.hpp>

namespace tmx = tmns::math;

/********************************************/
/*          Check To Vector Methods         */
/********************************************/
TEST( Vector_Utilities, ToVector_Methods )
{
    auto vec1 = tmns::math::ToVector2<double>( 0, 1 );
    ASSERT_NEAR( vec1.x(), 0, 0.0001 );
    ASSERT_NEAR( vec1.y(), 1, 0.0001 );

    auto vec2 = tmns::math::ToVector3<double>( 0, 1, -2);
    ASSERT_NEAR( vec2.x(), 0, 0.0001 );
    ASSERT_NEAR( vec2.y(), 1, 0.0001 );
    ASSERT_NEAR( vec2.z(),-2, 0.0001 );
}

/*************************************/
/*       Test Vector Addition        */
/*************************************/
TEST( Vector_Utilities, vector_addition )
{
    tmns::math::Vector3d vec_01 ( { -1, 2.1, 0.5 } );
    tmns::math::Vector3d vec_02 ( {  8, 5.9, 8.5 } );

    auto vec_03 = vec_01 + vec_02;

    // Verify the iterator works
    double exp_value = 7;
    for( auto it = vec_03.begin(); it != vec_03.end(); it++ )
    {
        ASSERT_NEAR( (*it), exp_value, 0.0001 );
        exp_value += 1;
    }
}

/****************************************************/
/*      Test Vector / Scalar Multiplication         */
/****************************************************/
TEST( Vector_Utilities, vector_scalar_multiplication )
{
    //  Create test inputs
    tmns::math::Vector3d vec_01 ( { -1, 2.1, 0.5 } );
    double scalar_01 = 3;

    // Test Scalar x Vector
    auto res_01 = vec_01 * scalar_01;
    ASSERT_NEAR( res_01[0], -3, 0.001 );
    ASSERT_NEAR( res_01[1], 6.3, 0.001 );
    ASSERT_NEAR( res_01[2], 1.5, 0.001 );

    // Test Vector x Scalar
    tmns::math::Vector3d res_02 = scalar_01 * vec_01;
    ASSERT_NEAR( res_02.x(), -3, 0.001 );
    ASSERT_NEAR( res_02.y(), 6.3, 0.001 );
    ASSERT_NEAR( res_02.z(), 1.5, 0.001 );
}

/****************************************************/
/*      Test Vector / Scalar Division         */
/****************************************************/
TEST( Vector_Utilities, vector_scalar_division )
{
    //  Create test inputs
    tmns::math::Vector3d vec_01 ( { -1, 2.2, 0.8 } );
    double scalar_01 = 2;

    // Test Scalar x Vector
    auto res_01 = vec_01 / scalar_01;
    ASSERT_NEAR( res_01[0], -0.5, 0.001 );
    ASSERT_NEAR( res_01[1], 1.1, 0.001 );
    ASSERT_NEAR( res_01[2], 0.4, 0.001 );
}

/****************************************/
/*      Test Vector Normalization       */
/****************************************/
TEST( Vector_Utilities, normalize_l2 )
{
    // Test 1, normal vector
    tmns::math::Vector3f vec01( { 1, 2, 3 } );

    auto res_01 = tmx::normalize( vec01 );
    ASSERT_NEAR( res_01[0], 0.267, 0.001 );
    ASSERT_NEAR( res_01[1], 0.535, 0.001 );
    ASSERT_NEAR( res_01[2], 0.802, 0.001 );

}