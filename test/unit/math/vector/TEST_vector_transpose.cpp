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
 * @file    TEST_vector_transpose.cpp
 * @author  Marvin Smith
 * @date    10/14/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/math/vector/Vector_Transpose.hpp>

namespace tmx = tmns::math;

class Vector_Transpose : public ::testing::Test
{
    protected:

        void SetUp() override
        {

        }

        /// @brief Underlying First Matrix Data
        std::array<double,3> data_01 { 1, 2, 3 };

        /// @brief Underlying Vector
        tmx::Vector3d vector_01 { data_01 };
};

/********************************/
/*      Perform Transpose       */
/********************************/
TEST_F( Vector_Transpose, functional_test )
{
    // Verify results
    ASSERT_NEAR( vector_01[0], 1, 0.001 );
    ASSERT_NEAR( vector_01[1], 2, 0.001 );
    ASSERT_NEAR( vector_01[2], 3, 0.001 );

    // Create transpose
    auto result = tmx::transpose( vector_01 );

    ASSERT_NEAR( result[0], 1, 0.001 );
    ASSERT_NEAR( result[1], 2, 0.001 );
    ASSERT_NEAR( result[2], 3, 0.001 );
}
