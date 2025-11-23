/**
 * @file    TEST_Vector.cpp
 * @author  Marvin Smith
 * @date    7/23/2023
*/
#include <gtest/gtest.h>

// Terminus Math Libraries
#include <terminus/math/vector/Vector.hpp>

/************************************/
/*      Test Simple Operations      */
/************************************/
TEST( Vector, Simple_Operations )
{
    // Check the default constructor
    tmns::math::Vector3i v1;
    ASSERT_EQ( v1.x(), 0 );
    ASSERT_EQ( v1.y(), 0 );
    ASSERT_EQ( v1.z(), 0 );

    // Check the fill constructor
    auto v2 = tmns::math::Vector3i( 3 );
    ASSERT_EQ( v2.x(), 3 );
    ASSERT_EQ( v2.y(), 3 );
    ASSERT_EQ( v2.z(), 3 );
}

/**************************************/
/*      Test the Array Constructors   */
/**************************************/
TEST( Vector, Constructors )
{
    // Create an array 
    std::array<double,3> arr01 { 1, 2, 3 };
    std::vector<double> arr02 { 1, 2, 3 };

    // Create Vector
    tmns::math::Vector3d vec01( arr01 );
    ASSERT_NEAR( vec01[0], 1, 0.001 );
    ASSERT_NEAR( vec01[1], 2, 0.001 );
    ASSERT_NEAR( vec01[2], 3, 0.001 );

    // Create another vector of different type
    tmns::math::Vector3i vec02( arr01 );
    ASSERT_NEAR( vec02[0], 1, 0.001 );
    ASSERT_NEAR( vec02[1], 2, 0.001 );
    ASSERT_NEAR( vec02[2], 3, 0.001 );

    // Construct Vector using std::vector
    tmns::math::Vector3d vec03( arr02 );
    ASSERT_NEAR( vec03[0], 1, 0.001 );
    ASSERT_NEAR( vec03[1], 2, 0.001 );
    ASSERT_NEAR( vec03[2], 3, 0.001 );

}

/************************************************/
/*          Test the Normalize Function         */
/************************************************/
TEST( Vector, normalize )
{
    tmns::math::Vector3f vec01( { 1, 2, 3 } );

    auto res_01 = vec01.normalize();
    ASSERT_NEAR( res_01[0], 0.267, 0.001 );
    ASSERT_NEAR( res_01[1], 0.535, 0.001 );
    ASSERT_NEAR( res_01[2], 0.802, 0.001 );
}

/****************************************/
/*          Test the Dot-Product        */
/****************************************/
TEST( Vector, dot )
{
    // Create 2 vectors
    tmns::math::Vector3f vec_01( {  1, 2, 3 } );
    tmns::math::Vector3f vec_02( { -1, 2, 4 } );

    // Compute the dot-product
    auto dp_01 = tmns::math::Vector3f::dot( vec_01, vec_02 );

    ASSERT_NEAR( dp_01, 15, 0.001 );
}
