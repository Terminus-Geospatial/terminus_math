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
 * @file    axis.hpp
 * @author  Marvin Smith
 * @date    11/15/2026
*/
#pragma once

// C++ Standard Libraries
#include <cstddef>
#include <string>
#include <vector>

namespace tmns::geo {

/**
 * @brief Cardinal direction of a coordinate axis.
 */
enum class Axis_Direction {

    UNSPECIFIED,
    EAST,        ///< Longitude / easting increases.
    WEST,
    NORTH,       ///< Latitude / northing increases.
    SOUTH,
    UP,          ///< Height/elevation increases.
    DOWN,
    OTHER        ///< Non-geographic axis (e.g. engineering frames).

}; // End of Axis_Direction enum


/**
 * @brief One coordinate axis: name, direction, and unit.
 */
struct Axis {

    std::string    name;
    Axis_Direction direction = Axis_Direction::UNSPECIFIED;
    std::string    unit;

}; // End of Axis struct


/**
 * @brief Declared axis set for a CRS.
 *
 * Axis order is explicit rather than assumed — EPSG:4326 is authority
 * lat/lon but is universally treated as lon/lat.  `is_traditional_gis_order`
 * is true when the first axis is EAST (x/y ordering).
 */
class Axis_Set
{
    public:

        /// lon/lat in degrees (traditional GIS order).
        static Axis_Set lon_lat_degrees();

        /// lat/lon in degrees (authority order).
        static Axis_Set lat_lon_degrees();

        /// easting/northing in metres (projected grids).
        static Axis_Set easting_northing_meters();

        /// X/Y/Z in metres (geocentric).
        static Axis_Set ecef_meters();

        /**
         * @brief Build from explicit axes.
         */
        explicit Axis_Set( std::vector<Axis> axes = {} );

        /**
         * @brief Number of axes (coordinate dimension).
         */
        size_t size() const;

        /**
         * @brief Axis access.
         */
        const Axis& operator[]( size_t index ) const;

        /**
         * @brief True when the first axis runs EAST (lon,lat ordering).
         */
        bool is_traditional_gis_order() const;

    private:

        std::vector<Axis> m_axes;

}; // End of Axis_Set Class

} // end of tmns::geo namespace
