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
 * @file    TEST_size.cpp
 * @author  Marvin Smith
 * @date    7/23/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/math/size.hpp>


/****************************************/
/*          Basic Constructor Test      */
/****************************************/
TEST( Size, Basic_Construction_Tests )
{
    auto sz1 = tmns::math::Size2i();
    ASSERT_EQ( sz1.width(), 0 );
    ASSERT_EQ( sz1.height(), 0 );

    auto sz2 = tmns::math::Size3f();
    ASSERT_NEAR( sz2.width(), 0, 0.001 );
    ASSERT_NEAR( sz2.height(), 0, 0.001 );
    ASSERT_NEAR( sz2.depth(), 0, 0.001 );
}