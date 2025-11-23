/**
 * @file    TEST_Matrix.cpp
 * @author  Marvin Smith
 * @date    9/16/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/math/matrix/MatrixN.hpp>

namespace tmx = tmns::math;

/****************************************/
/*      Test the Matrix Constructor     */
/****************************************/
TEST( MatrixN, constructor_tests )
{
    //////////////////////////////////////////
    //  Test 1:  2x3 matrix constructor
    //////////////////////////////////////////
    tmx::MatrixN<double> mat_01( 2, 3 );
    ASSERT_EQ( mat_01.cols(), 3 );
    ASSERT_EQ( mat_01.rows(), 2 );

    int counter = 0;
    for( auto it = mat_01.begin();
         it != mat_01.end(); 
         it++ )
    {
        ASSERT_NEAR( (*it), 0, 0.001 );
        (*it) = counter++;
    }

    //////////////////////////////////////////
    //  Test 2:  2x2 matrix constructor
    //////////////////////////////////////////
    std::array<float,4> data = { 1, 2, 3, 4 };
    tmx::MatrixN<float> mat_02( 2, 2, data.data() );

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
    tmx::MatrixN<float> mat_03( mat_02 );

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
    tmx::MatrixN<float> mat_04( 2, 2, { 1.f, 2.f, 3.f, 4.f } );

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
    ASSERT_EQ( &(*(mat_01.begin())),   &(mat_01(0,0)));
    ASSERT_EQ( &(*(mat_01.begin()+1)), &(mat_01(0,1)));
    ASSERT_EQ( mat_01.end(), mat_01.begin() + 6 );
}