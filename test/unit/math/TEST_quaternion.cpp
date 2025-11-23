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
 * @file    TEST_quaternion.cpp
 * @author  Marvin Smith
 * @date    9/15/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/math/quaternion.hpp>
#include <terminus/math/vector/vector_utilities.hpp>

/************************************************/
/*      Test the Quaternion Constructors        */
/************************************************/
TEST( Quaternion, Constructors )
{
    tmns::math::Quaternion q1;
    ASSERT_NEAR( q1.real(),     0, 0.001 );
    ASSERT_NEAR( q1.imag().x(), 1, 0.001 );
    ASSERT_NEAR( q1.imag().y(), 0, 0.001 );
    ASSERT_NEAR( q1.imag().z(), 0, 0.001 );

}

/****************************************/
/*      Test the Quaternion Inverse     */
/****************************************/
TEST( Quaternion, Inverse )
{
    tmns::math::Quaternion q1;

    auto inv = q1.inverse();
}

