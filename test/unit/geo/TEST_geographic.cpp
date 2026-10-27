/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/*                                                                                    */
/*                           Copyright (c) 2026 Terminus LLC                          */
/*                                                                                    */
/*                                All Rights Reserved.                                */
/*                                                                                    */
/*          Use of this source code is governed by LICENSE in the repo root.          */
/*                                                                                    */
/***************************# INTELLECTUAL PROPERTY RIGHTS ****************************/
/**
 * @file    TEST_geographic.cpp
 * @author  Marvin Smith
 * @date    10/26/2026
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/geo/coord/geographic.hpp>
#include <terminus/geo/datum.hpp>

// Test Data
#include <geo/geo_test_data.hpp>

using namespace tmns::geo;
using namespace tmns::geo::coord;
using namespace tmns::geo::test;

/****************************************/
/*     Factory Validation (Table)       */
/****************************************/
TEST( Geographic_Test, from_degrees_validates_inputs )
{
    for( const auto& test_case : FACTORY_CASES )
    {
        auto result = Geographic::from_degrees( test_case.lat_deg,
                                                test_case.lon_deg,
                                                test_case.elev_m );
        if( test_case.expect_success )
        {
            ASSERT_FALSE( result.has_error() ) << "case: " << test_case.name;
            EXPECT_NEAR( result.value().latitude_degrees(),  test_case.lat_deg, 1e-9 ) << test_case.name;
            EXPECT_NEAR( result.value().longitude_degrees(), test_case.lon_deg, 1e-9 ) << test_case.name;
        }
        else
        {
            EXPECT_TRUE( result.has_error() ) << "case: " << test_case.name;
        }
    }
}

/****************************************/
/*         Distance (Table)             */
/****************************************/
TEST( Geographic_Test, distance_known_values )
{
    for( const auto& test_case : DISTANCE_CASES )
    {
        auto coord1 = Geographic::from_degrees( test_case.lat1_deg, test_case.lon1_deg );
        auto coord2 = Geographic::from_degrees( test_case.lat2_deg, test_case.lon2_deg );
        ASSERT_FALSE( coord1.has_error() ) << "case: " << test_case.name;
        ASSERT_FALSE( coord2.has_error() ) << "case: " << test_case.name;

        auto distance = Geographic::distance( coord1.value(), coord2.value(), test_case.method );
        ASSERT_FALSE( distance.has_error() ) << "case: " << test_case.name;
        EXPECT_NEAR( distance.value(), test_case.expected_m, test_case.tolerance_m )
            << "case: " << test_case.name;
    }
}

/****************************************/
/*       Datum Mismatch Rejected        */
/****************************************/
TEST( Geographic_Test, distance_rejects_datum_mismatch )
{
    auto coord1 = Geographic::from_degrees( 40.0, -105.0, 0.0, Datum::WGS84() ).value();
    auto coord2 = Geographic::from_degrees( 40.5, -105.5, 0.0, Datum::SPHERE() ).value();

    auto result = Geographic::distance( coord1, coord2 );
    EXPECT_TRUE( result.has_error() );
    EXPECT_EQ( result.error().code(), tmns::error::Error_Code::INVALID_CONFIGURATION );
}

/****************************************/
/*           To String                  */
/****************************************/
TEST( Geographic_Test, to_string_formats )
{
    auto coord = Geographic::from_degrees( 40.0, -105.0, 100.0 ).value();

    EXPECT_NE( coord.to_string().find( "40" ), std::string::npos );
    EXPECT_NE( coord.to_string( "google" ).find( ", " ), std::string::npos );
    EXPECT_THROW( coord.to_string( "bogus" ), std::invalid_argument );
}

/****************************************/
/*           Base Interface             */
/****************************************/
TEST( Geographic_Test, base_interface )
{
    auto coord = Geographic::from_degrees( 40.0, -105.0, 100.0 ).value();

    EXPECT_FALSE( coord.is_projected() );
    auto cloned = coord.clone();
    EXPECT_NEAR( static_cast<Geographic*>( cloned.get() )->latitude_degrees(), 40.0, 1e-9 );
}
