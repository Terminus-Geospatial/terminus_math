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
 * @file    base.hpp
 * @author  Marvin Smith
 * @date    10/26/2026
*/
#pragma once

// C++ Standard Libraries
#include <memory>
#include <string>

namespace tmns::geo::coord {

/**
 * @brief Base class for all coordinate types.
 *
 * All coordinate types store their components internally as a 3-element
 * array and expose x/y/z accessors whose meaning depends on the concrete
 * system (e.g. Geographic: lon/lat/elev; UTM: easting/northing/elev).
 */
class Base
{
    public:

        /// @brief Destructor.
        virtual ~Base() = default;

        /**
         * @brief First coordinate component.
         *
         * Meaning depends on the coordinate type:
         *  - Geographic: Longitude (radians)
         *  - UTM:        Easting (meters)
         *  - ECEF:       X coordinate (meters)
         */
        virtual double x() const = 0;

        /**
         * @brief Second coordinate component.
         *
         *  - Geographic: Latitude (radians)
         *  - UTM:        Northing (meters)
         *  - ECEF:       Y coordinate (meters)
         */
        virtual double y() const = 0;

        /**
         * @brief Third coordinate component.
         *
         *  - Geographic: Elevation (meters)
         *  - UTM:        Elevation (meters)
         *  - ECEF:       Z coordinate (meters)
         */
        virtual double z() const = 0;

        /**
         * @brief Whether this coordinate uses a projected coordinate system.
         *
         * Projected systems (UTM, etc.) have Euclidean XY distances, which is
         * useful for pixel-space and iterative intersection algorithms.
         */
        virtual bool is_projected() const = 0;

        /**
         * @brief Create a deep copy of this coordinate.
         */
        virtual std::unique_ptr<Base> clone() const = 0;

        /**
         * @brief String representation.
         * @param format  Output style defined by the concrete type
         *                (default: "pretty").
         */
        virtual std::string to_string( const std::string& format = "pretty" ) const = 0;

}; // End of Base Class

} // end of tmns::geo::coord namespace
