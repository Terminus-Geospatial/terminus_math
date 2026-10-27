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
 * @file    ecef.hpp
 * @author  Marvin Smith
 * @date    10/26/2026
*/
#pragma once

// C++ Standard Libraries
#include <array>
#include <memory>
#include <string>

// Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/outcome.hpp>
#include <terminus/geo/coord/base.hpp>
#include <terminus/geo/datum.hpp>

namespace tmns::geo::coord {

/**
 * @brief Earth-Centered, Earth-Fixed (ECEF) coordinate.
 *
 * Cartesian position in meters within a rotating, geocentric frame tied to
 * the horizontal datum's ellipsoid.  ECEF is the canonical exchange
 * representation between geodetic systems.
 */
class ECEF : public Base
{
    public:

        /**
         * @brief Create an ECEF coordinate.
         *
         * @param x_m                X component in meters.
         * @param y_m                Y component in meters.
         * @param z_m                Z component in meters.
         * @param horizontal_datum   Reference ellipsoid (default: WGS84).
         *
         * @return Coordinate on success; INVALID_INPUT when components are
         *         non-finite or the position falls inside the ellipsoid
         *         interior (magnitude < semi-minor axis).
         */
        static Result<ECEF> create( double       x_m,
                                    double       y_m,
                                    double       z_m,
                                    const Datum& horizontal_datum = Datum::WGS84() );

        /// @brief X component (meters).
        double x() const override { return m_data[0]; }

        /// @brief Y component (meters).
        double y() const override { return m_data[1]; }

        /// @brief Z component (meters).
        double z() const override { return m_data[2]; }

        void set_x( double x_m ) { m_data[0] = x_m; }
        void set_y( double y_m ) { m_data[1] = y_m; }
        void set_z( double z_m ) { m_data[2] = z_m; }

        /// @brief Reference ellipsoid.
        const Datum& horizontal_datum() const { return m_horizontal_datum; }

        /**
         * @brief ECEF is a geocentric Cartesian frame, not a map projection.
         */
        bool is_projected() const override { return false; }

        std::unique_ptr<Base> clone() const override;

        std::string to_string( const std::string& format = "pretty" ) const override;

        /**
         * @brief Euclidean distance to another coordinate, in meters.
         */
        Result<double> distance_to( const ECEF& other ) const;

        /**
         * @brief Euclidean distance between two ECEF coordinates, in meters.
         */
        static Result<double> distance( const ECEF& coord1,
                                        const ECEF& coord2 );

    private:

        /**
         * @brief Private constructor — use create() for validated
         *        construction.
         */
        ECEF( double x_m, double y_m, double z_m, const Datum& horizontal_datum )
            : m_data{ x_m, y_m, z_m },
              m_horizontal_datum( horizontal_datum )
        {
        }

        /// Coordinate components: [x, y, z] in meters.
        std::array<double, 3> m_data{ 0.0, 0.0, 0.0 };

        /// Reference ellipsoid.
        Datum m_horizontal_datum;

}; // End of ECEF Class

} // end of tmns::geo::coord namespace
