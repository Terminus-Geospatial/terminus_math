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
 * @file    TEST_eigen_utilities.cpp
 * @author  Marvin Smith
 * @date    3/9/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/math/matrix.hpp>
#include "../../../../../src/math/thirdparty/eigen/Eigen_Utilities.hpp"

using namespace tmns::math;

/**
 *  Test the To/From Eigen (Matrix)
*/
TEST( thirdparty_eigen_utilities, to_from_eigen_matrix_float_01 )
{
    // Create a Terminus Matrix
    MatrixN<float> A( 4, 4, {  23,   1,  25,  98,
                              327,   2,  76,  66,
                              234,  26,  76, 662,
                               25,  62, 323,  23 } );

    // Convert the Terminus matrix into an Eigen type
    auto resA = eigen::to_eigen<::Eigen::MatrixXf>( A );
    ::Eigen::MatrixXf eigenA = resA.value();

    // Convert back to
    auto outResA = eigen::from_eigen<MatrixN<float>>( eigenA );
    MatrixN<float> B = outResA.value();

    auto delta = A - B;
    ASSERT_NEAR( delta.sum(), 0, 0.001 );
}

/********************************************************/
/*      Convert Matrices back and forth from Eigen      */
/********************************************************/
TEST( thirdparty_eigen_utilities, to_from_eigen_matrix_float_02 )
{
    // Create a Terminus Matrix
    MatrixN<float> A( 3, 4, {  23,  1, 25,  98,
                                327,  2, 76,  66,
                                234, 26, 76, 662 } );

    // Convert the Terminus matrix into an Eigen type
    auto resA = eigen::to_eigen<::Eigen::MatrixXf>( A );
    ::Eigen::MatrixXf eigenA = resA.value();

    // Convert back to
    auto outResA = eigen::from_eigen<MatrixN<float>>( eigenA );
    MatrixN<float> B = outResA.value();

    auto delta = A - B;
    ASSERT_NEAR( delta.sum(), 0, 0.001 );
}
