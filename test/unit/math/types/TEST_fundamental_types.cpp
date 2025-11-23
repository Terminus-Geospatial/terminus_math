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
 * @file    TEST_fundamental_types.cpp
 * @author  Marvin Smith
 * @date    7/23/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/math/types/Fundamental_Types.hpp>

/******************************************/
/*      Test the Is-Scalar Type-Trait     */
/******************************************/
TEST( Fundamental_Types, Is_Scalar )
{
    ASSERT_TRUE( tmns::math::Is_Scalar<float>::value );
    ASSERT_TRUE( tmns::math::Is_Scalar<uint8_t>::value );
}
