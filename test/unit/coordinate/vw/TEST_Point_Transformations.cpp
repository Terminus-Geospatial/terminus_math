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
 * @file    TEST_Point_Transformations.cpp
 * @author  Marvin Smith
 * @date    10/15/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/coordinate/conversions/vw/Point_Transformations.hpp>
#include <terminus/math/vector.hpp>

/********************************************/
/*      Test XYZ <- -> LLA Conversions      */
/********************************************/
TEST( Point_Transformations, XYZ_LLA_Conversions )
{
    // Test a full forward and reverse transformation
    tmns::math::Vector3d xyz( { -2197110.000000,
                                1741355.875000,
                                1898886.875000 } );
    auto lon_lat_alt = tmns::coordinate::vw::xyz_to_lon_lat_radius_estimate( xyz );
    ASSERT_FALSE( lon_lat_alt.has_error() );

    auto xyz2 = tmns::coordinate::vw::lon_lat_radius_to_xyz_estimate( lon_lat_alt.value() );
    ASSERT_FALSE( xyz2.has_error() );

    ASSERT_NEAR( tmns::math::VectorN<double>(( xyz - xyz2.value() )).magnitude(), 0, 0.001 );

    // Test to see if things still work for West positive coordinate
    // systems.
    lon_lat_alt = tmns::coordinate::vw::xyz_to_lon_lat_radius_estimate( xyz, false );
    ASSERT_FALSE( lon_lat_alt.has_error() );

    xyz2        = tmns::coordinate::vw::lon_lat_radius_to_xyz_estimate( lon_lat_alt.value(), false );
    ASSERT_NEAR( tmns::math::VectorN<double>(( xyz - xyz2.value() )).magnitude(), 0, 0.001 );

    // See if it still works if using 0-360 range
    xyz[1] = -xyz[1];
    lon_lat_alt = tmns::coordinate::vw::xyz_to_lon_lat_radius_estimate( xyz, true, false );
    ASSERT_FALSE( lon_lat_alt.has_error() );

    xyz2  = tmns::coordinate::vw::lon_lat_radius_to_xyz_estimate( lon_lat_alt.value() );
    ASSERT_FALSE( xyz2.has_error() );

    ASSERT_NEAR( tmns::math::VectorN<double>(( xyz - xyz2.value() )).magnitude(), 0, 0.001 );
}

