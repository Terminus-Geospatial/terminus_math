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
 * @file    TEST_matrix.cpp
 * @author  Marvin Smith
 * @date    9/16/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/math/matrix/matrix.hpp>

namespace tmx = tmns::math;

/****************************************/
/*      Test the Matrix Constructor     */
/****************************************/
TEST( Matrix, constructor_tests )
{
    //////////////////////////////////////////
    //  Test 1:  3x3 matrix constructor
    //////////////////////////////////////////
    tmx::Matrix<double,3,3> mat_01;
    ASSERT_EQ( mat_01.cols(), 3 );
    ASSERT_EQ( mat_01.rows(), 3 );

    for( auto it = mat_01.begin();
         it != mat_01.end();
         it++ )
    {
        ASSERT_NEAR( (*it), 0, 0.001 );
    }

    //////////////////////////////////////////
    //  Test 2:  2x2 matrix constructor
    //////////////////////////////////////////
    std::array<float,4> data = { 1, 2, 3, 4 };
    tmx::Matrix<float,2,2> mat_02( data );

    ASSERT_EQ( 2, mat_02.rows() );
    ASSERT_EQ( 2, mat_02.cols() );

    ASSERT_EQ( 1, mat_02(0, 0) );
    ASSERT_EQ( 2, mat_02(0, 1) );
    ASSERT_EQ( 3, mat_02(1, 0) );
    ASSERT_EQ( 4, mat_02(1, 1) );

    ASSERT_EQ( 1, mat_02[0][0] );
    ASSERT_EQ( 2, mat_02[0][1] );
    ASSERT_EQ( 3, mat_02[1][0] );
    ASSERT_EQ( 4, mat_02[1][1] );

    //////////////////////////////////////////
    //  Test 3:  Copy Constructor
    //////////////////////////////////////////
    tmx::Matrix<float,2,2> mat_03( mat_02 );

    ASSERT_EQ( 2, mat_03.rows() );
    ASSERT_EQ( 2, mat_03.cols() );

    ASSERT_EQ( 1, mat_03(0, 0) );
    ASSERT_EQ( 2, mat_03(0, 1) );
    ASSERT_EQ( 3, mat_03(1, 0) );
    ASSERT_EQ( 4, mat_03(1, 1) );

    ASSERT_EQ( 1, mat_03[0][0] );
    ASSERT_EQ( 2, mat_03[0][1] );
    ASSERT_EQ( 3, mat_03[1][0] );
    ASSERT_EQ( 4, mat_03[1][1] );

    //////////////////////////////////////////
    //  Test 4:  Element Constructor
    //////////////////////////////////////////
    tmx::Matrix<float,2,2> mat_04( { 1.f, 2.f, 3.f, 4.f } );

    ASSERT_EQ( 2, mat_04.rows() );
    ASSERT_EQ( 2, mat_04.cols() );

    ASSERT_EQ( 1, mat_04(0, 0) );
    ASSERT_EQ( 2, mat_04(0, 1) );
    ASSERT_EQ( 3, mat_04(1, 0) );
    ASSERT_EQ( 4, mat_04(1, 1) );

    // Verify resize works
    EXPECT_NO_THROW( mat_01.set_size(3,3) );
    EXPECT_NO_THROW( mat_01.set_size(2,3) );

    // Iterators
    ASSERT_EQ( &(*(mat_01.begin())),&(mat_01(0,0)));
    ASSERT_EQ( &(*(mat_01.begin()+1)), &(mat_01(0,1)));
    ASSERT_EQ( mat_01.end(), mat_01.begin() + 9 );

    //////////////////////////////////////////
    //  Test 5:  2x2 matrix constructor
    //////////////////////////////////////////
    std::vector<float> data5 = { 1, 2, 3, 4 };
    tmx::Matrix<float,2,2> mat_05( data5 );

    ASSERT_EQ( 2, mat_05.rows() );
    ASSERT_EQ( 2, mat_05.cols() );

    ASSERT_EQ( 1, mat_05(0, 0) );
    ASSERT_EQ( 2, mat_05(0, 1) );
    ASSERT_EQ( 3, mat_05(1, 0) );
    ASSERT_EQ( 4, mat_05(1, 1) );

    ASSERT_EQ( 1, mat_05[0][0] );
    ASSERT_EQ( 2, mat_05[0][1] );
    ASSERT_EQ( 3, mat_05[1][0] );
    ASSERT_EQ( 4, mat_05[1][1] );
}

/****************************************/
/*          Test the Sum Method         */
/****************************************/
TEST( Matrix, sum )
{
    std::array<double,12> data = {  23, -1,  25,   98,
                                   327,  2, 0.1,   66,
                                   234, 26,  76, -662 };
    tmx::Matrix<double,3,4> A( data );

    ASSERT_NEAR( A.sum(), 214.1, 0.001 );
}