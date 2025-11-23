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
 * @file    TEST_matrix_transpose.cpp
 * @author  Marvin Smith
 * @date    10/10/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/math/matrix.hpp>

// C++ Libraries
#include <numeric>
#include <ranges>

namespace tmx = tmns::math;

class Matrix_Transpose : public ::testing::Test
{
    protected:

        void SetUp() override
        {
            // Create matrix
            std::iota( data.begin(), data.end(), 0 );

            // Create matrix
            test_mat_01 = tmx::Matrix<double,3,4>( data );
        }

        /// Data underneath matrix
        std::array<double,12> data;

        /// Matrix prior to transpose
        tmx::Matrix<double,3,4> test_mat_01;

        /// Expected Result
        std::array<double,12> exp_trans_data { 0, 4, 8, 1, 5, 9, 2, 6, 10, 3, 7, 11 };

}; // End of Matrix_Transpose class

/************************************************/
/*          Test the Standard Transpose         */
/************************************************/
TEST_F( Matrix_Transpose, transpose )
{

    // Verify non transpose shape
    size_t counter = 0;
    for( int r = 0; r < test_mat_01.rows(); r++ )
    for( int c = 0; c < test_mat_01.cols(); c++ )
    {
        ASSERT_NEAR( test_mat_01[r][c], data[counter++], 0.001 );
    }

    // Verify transpose
    counter = 0;
    auto mat_trans_01 = tmx::transpose( test_mat_01 );
    ASSERT_EQ( mat_trans_01.rows(), 4 );
    ASSERT_EQ( mat_trans_01.cols(), 3 );
    for( int r = 0; r < mat_trans_01.rows(); r++ )
    for( int c = 0; c < mat_trans_01.cols(); c++ )
    {
        ASSERT_NEAR( mat_trans_01[r][c], exp_trans_data[counter++], 0.001 );
    }

    // Assign to a real matrix
    tmx::Matrix<double,4,3> new_mat = mat_trans_01;

    // Verify it's the same
    counter = 0;
    ASSERT_EQ( new_mat.rows(), 4 );
    ASSERT_EQ( new_mat.cols(), 3 );
    for( int r = 0; r < new_mat.rows(); r++ )
    for( int c = 0; c < new_mat.cols(); c++ )
    {
        ASSERT_NEAR( new_mat[r][c], exp_trans_data[counter++], 0.001 );
    }

    // Compare using the mat's .transpose() operator
    counter = 0;
    auto final_mat = new_mat.T();
    ASSERT_EQ( final_mat.rows(), 3 );
    ASSERT_EQ( final_mat.cols(), 4 );
    for( int r = 0; r < final_mat.rows(); r++ )
    for( int c = 0; c < final_mat.cols(); c++ )
    {
        ASSERT_NEAR( final_mat[r][c], data[counter++], 0.001 );
    }
}

/************************************************/
/*          Test the Standard Transpose         */
/************************************************/
TEST_F( Matrix_Transpose, iterator )
{
    auto mat_trans_01 = tmx::transpose( test_mat_01 );

    size_t counter = 0;
    for( auto it = mat_trans_01.begin(); it != mat_trans_01.end(); it++ )
    {
        ASSERT_NEAR( (*it), exp_trans_data[counter++], 0.001 );
    }
}