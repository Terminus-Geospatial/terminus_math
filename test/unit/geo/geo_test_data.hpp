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
 * @file    geo_test_data.hpp
 * @author  Marvin Smith
 * @date    10/26/2026
 *
 * @brief   Table-driven test data for geo coordinate tests.
 *
 * Each group is a vector of case structs.  Tests iterate a group rather
 * than hard-coding one scenario per TEST(), so adding coverage is a
 * one-line append to the data table.  New groups (e.g. coordinate
 * conversions when ECEF/UTM land) follow the same pattern.
*/
#pragma once

// C++ Standard Libraries
#include <limits>
#include <string>
#include <vector>

// Terminus Libraries
#include <terminus/geo/distance_method_enum.hpp>

namespace tmns::geo::test {

/**
 * @brief A single great-circle distance expectation.
 *
 * Expected values are verified against GeographicLib (WGS-84):
 *  - 1 degree of latitude at the equator       : 110,574.39 m (Vincenty)
 *  - 1 degree of longitude at the equator      : 111,319.49 m (Vincenty)
 *  - 1 degree of longitude at 45 N             :  78,846.33 m (Vincenty)
 *  - Denver, CO to New York, NY                : 2,625,638.76 m
 *  - Haversine equivalents use the 6,371,000 m mean radius, giving
 *    111,194.93 m per degree on the sphere.
 */
struct Distance_Case
{
    std::string      name;
    double           lat1_deg;
    double           lon1_deg;
    double           lat2_deg;
    double           lon2_deg;
    Distance_Method  method;
    double           expected_m;
    double           tolerance_m;
};

/// @brief Distance expectations; append a row for a new scenario.
inline const std::vector<Distance_Case> DISTANCE_CASES = {
    { "same_point",            40.0, -105.0,  40.0, -105.0, Distance_Method::HAVERSINE,      0.0,      1e-6 },
    { "one_deg_lat_equator",    0.0,    0.0,   1.0,    0.0, Distance_Method::HAVERSINE, 111194.93,     10.0 },
    { "one_deg_lon_equator",    0.0,    0.0,   0.0,    1.0, Distance_Method::HAVERSINE, 111194.93,     10.0 },
    { "pole_to_equator",       90.0,    0.0,   0.0,    0.0, Distance_Method::HAVERSINE, 10007542.0,   5000.0 },
    { "vincenty_lat_equator",   0.0,    0.0,   1.0,    0.0, Distance_Method::VINCENTY,  110574.39,      0.05 },
    { "vincenty_lon_equator",   0.0,    0.0,   0.0,    1.0, Distance_Method::VINCENTY,  111319.49,      0.05 },
    { "vincenty_lon_45n",      45.0,    0.0,  45.0,    1.0, Distance_Method::VINCENTY,   78846.33,      0.05 },
    { "vincenty_denver_nyc",   39.7392, -104.9903, 40.7128, -74.006, Distance_Method::VINCENTY, 2625638.76, 0.10 },
    { "same_point_vincenty",   40.0, -105.0,  40.0, -105.0, Distance_Method::VINCENTY,        0.0,      1e-6 },
};

/**
 * @brief A single from_degrees() factory expectation.
 */
struct Factory_Case
{
    std::string name;
    double      lat_deg;
    double      lon_deg;
    double      elev_m;
    bool        expect_success;
};

/// @brief Factory validation cases; append a row for a new scenario.
inline const std::vector<Factory_Case> FACTORY_CASES = {
    { "denver",          39.7392, -104.9903, 1609.0, true  },
    { "north_pole",      90.0,       0.0,       0.0, true  },
    { "south_pole",     -90.0,       0.0,       0.0, true  },
    { "date_line_east",   0.0,     180.0,       0.0, true  },
    { "date_line_west",   0.0,    -180.0,       0.0, true  },
    { "lat_too_high",    90.01,      0.0,       0.0, false },
    { "lat_too_low",    -90.01,      0.0,       0.0, false },
    { "lon_too_high",     0.0,     180.01,      0.0, false },
    { "lon_too_low",      0.0,    -180.01,      0.0, false },
    { "nan_lat",         std::numeric_limits<double>::quiet_NaN(), 0.0, 0.0, false },
    { "inf_lon",         0.0, std::numeric_limits<double>::infinity(), 0.0, false },
};

} // end of tmns::geo::test namespace
