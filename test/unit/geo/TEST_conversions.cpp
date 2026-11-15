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
 * @file    TEST_conversions.cpp
 * @author  Marvin Smith
 * @date    11/15/2026
 *
 * @brief   Round-trip and known-anchor tests for the closed-form
 *          geo::coord conversions.
 *
 * Anchors are definitional (equator/prime-meridian gives x = a, the
 * pole gives z = b) rather than duplicating the implementation's own
 * formulas.  Round-trips assert the inverse recovers the input within
 * float-safe tolerances.
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/geo/conversions.hpp>
#include <terminus/geo/datum.hpp>

using namespace tmns::geo;
using namespace tmns::geo::coord;

namespace {

const Datum WGS84 = Datum::WGS84();
constexpr double DEG_TOL  = 1e-9;    // ~0.1 mm at the surface
constexpr double UTM_TOL  = 1e-7;    // ~1 cm — e^6 series truncation
constexpr double ELEV_TOL = 1e-3;    // 1 mm

struct Geo_Point {
    double lat_deg;
    double lon_deg;
    double elev_m;
};

const Geo_Point GEO_ROUND_TRIP_POINTS[] = {
    {   0.0,      0.0,      0.0 },
    {  40.748817, -73.985428,  430.0 },
    { -33.8568,   151.2153,     58.0 },
    {  51.4778,    -0.0014,     46.0 },
    { -89.9,        0.0,     2800.0 },
    {  89.9,      179.9999,     0.0 }
};

} // end of anonymous namespace


/****************************************/
/*      geo -> ECEF anchors             */
/****************************************/
TEST( Conversions_Test, to_ecef_definitional_anchors )
{
    const double a = WGS84.semi_major_axis();
    const double b = WGS84.semi_minor_axis();

    // Equator + prime meridian: (a, 0, 0)
    auto p0 = Geographic::from_degrees( 0.0, 0.0, 0.0 );
    ASSERT_FALSE( p0.has_error() );
    auto e0 = to_ecef( p0.value() );
    ASSERT_FALSE( e0.has_error() );
    EXPECT_NEAR( e0.value().x(), a, 1e-3 );
    EXPECT_NEAR( e0.value().y(), 0.0, 1e-3 );
    EXPECT_NEAR( e0.value().z(), 0.0, 1e-3 );

    // North pole: (0, 0, b)
    auto pn = Geographic::from_degrees( 90.0, 0.0, 0.0 );
    ASSERT_FALSE( pn.has_error() );
    auto en = to_ecef( pn.value() );
    ASSERT_FALSE( en.has_error() );
    EXPECT_NEAR( en.value().x(), 0.0, 1e-3 );
    EXPECT_NEAR( en.value().y(), 0.0, 1e-3 );
    EXPECT_NEAR( en.value().z(), b, 1e-3 );
}


/****************************************/
/*      geo -> ECEF -> geo              */
/****************************************/
TEST( Conversions_Test, geographic_ecef_round_trip )
{
    for( const auto& pt : GEO_ROUND_TRIP_POINTS )
    {
        SCOPED_TRACE( pt.lat_deg );
        auto geo = Geographic::from_degrees( pt.lat_deg, pt.lon_deg, pt.elev_m );
        ASSERT_FALSE( geo.has_error() );

        auto ecef = to_ecef( geo.value() );
        ASSERT_FALSE( ecef.has_error() );

        auto back = to_geographic( ecef.value() );
        ASSERT_FALSE( back.has_error() );

        EXPECT_NEAR( back.value().latitude_degrees(),  pt.lat_deg, DEG_TOL );
        EXPECT_NEAR( back.value().longitude_degrees(), pt.lon_deg, DEG_TOL );
        EXPECT_NEAR( back.value().elevation_meters(),  pt.elev_m,  ELEV_TOL );
    }
}


/****************************************/
/*      geo -> UTM anchors              */
/****************************************/
TEST( Conversions_Test, to_utm_definitional_anchors )
{
    // Equator on a zone central meridian: easting = 500 km, northing = 0.
    // Zone 31 central meridian = 3 E.
    auto eq = Geographic::from_degrees( 0.0, 3.0, 0.0 );
    ASSERT_FALSE( eq.has_error() );
    auto u = to_utm( eq.value(), 31 );
    ASSERT_FALSE( u.has_error() );
    EXPECT_EQ( u.value().zone(), 31 );
    EXPECT_EQ( u.value().hemisphere(), Hemisphere::NORTH );
    EXPECT_NEAR( u.value().easting_meters(),  500000.0, 1e-3 );
    EXPECT_NEAR( u.value().northing_meters(), 0.0,      1e-3 );

    // Southern hemisphere gets the 10,000 km false northing.
    auto sh = Geographic::from_degrees( -1.0, 3.0, 0.0 );
    ASSERT_FALSE( sh.has_error() );
    auto us = to_utm( sh.value(), 31 );
    ASSERT_FALSE( us.has_error() );
    EXPECT_EQ( us.value().hemisphere(), Hemisphere::SOUTH );
    EXPECT_GT( us.value().northing_meters(), 9000000.0 );

    // Auto-zone: Denver (~-104.99) falls in zone 13.
    auto denver = Geographic::from_degrees( 39.7392, -104.9903, 1609.0 );
    ASSERT_FALSE( denver.has_error() );
    auto ud = to_utm( denver.value() );
    ASSERT_FALSE( ud.has_error() );
    EXPECT_EQ( ud.value().zone(), 13 );
    EXPECT_NEAR( ud.value().elevation_meters(), 1609.0, 1e-9 );
}


/****************************************/
/*      geo -> UTM -> geo               */
/****************************************/
TEST( Conversions_Test, geographic_utm_round_trip )
{
    for( const auto& pt : GEO_ROUND_TRIP_POINTS )
    {
        // Skip near-pole points: UTM is only valid to 84N/80S.
        if( std::fabs( pt.lat_deg ) > 84.0 )
        {
            continue;
        }
        SCOPED_TRACE( pt.lat_deg );
        auto geo = Geographic::from_degrees( pt.lat_deg, pt.lon_deg, pt.elev_m );
        ASSERT_FALSE( geo.has_error() );

        auto utm = to_utm( geo.value() );
        ASSERT_FALSE( utm.has_error() );

        auto back = to_geographic( utm.value() );
        ASSERT_FALSE( back.has_error() );

        EXPECT_NEAR( back.value().latitude_degrees(),  pt.lat_deg, UTM_TOL );
        EXPECT_NEAR( back.value().longitude_degrees(), pt.lon_deg, UTM_TOL );
        EXPECT_NEAR( back.value().elevation_meters(),  pt.elev_m,  ELEV_TOL );
    }
}


/****************************************/
/*      UTM explicit-zone round-trip    */
/****************************************/
TEST( Conversions_Test, utm_explicit_zone_round_trip )
{
    // Landsat test asset CRS: UTM 13N.
    const Geo_Point pts[] = {
        { 39.0, -105.0, 1500.0 },
        { 40.5, -104.0, 2000.0 }
    };
    for( const auto& pt : pts )
    {
        auto geo = Geographic::from_degrees( pt.lat_deg, pt.lon_deg, pt.elev_m );
        ASSERT_FALSE( geo.has_error() );

        auto utm = to_utm( geo.value(), 13 );
        ASSERT_FALSE( utm.has_error() );
        EXPECT_EQ( utm.value().zone(), 13 );

        auto back = to_geographic( utm.value() );
        ASSERT_FALSE( back.has_error() );
        EXPECT_NEAR( back.value().latitude_degrees(),  pt.lat_deg, UTM_TOL );
        EXPECT_NEAR( back.value().longitude_degrees(), pt.lon_deg, UTM_TOL );
    }
}
