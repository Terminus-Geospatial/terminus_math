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
 * @file    geographic.hpp
 * @author  Marvin Smith
 * @date    10/26/2026
*/
#pragma once

// C++ Standard Libraries
#include <array>
#include <cmath>
#include <memory>
#include <string>

// Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/outcome.hpp>
#include <terminus/geo/constants.hpp>
#include <terminus/geo/coord/base.hpp>
#include <terminus/geo/datum.hpp>
#include <terminus/geo/distance_method_enum.hpp>

namespace tmns::geo::coord {

/**
 * @brief Geographic coordinate (Longitude / Latitude / Elevation).
 *
 * Stores longitude and latitude internally in radians, matching the
 * convention used across geodesy libraries, with degrees/radians accessors
 * on both.  Elevation is stored in meters above the horizontal datum.
 */
class Geographic : public Base
{
    public:

        /**
         * @brief Create a geographic coordinate from radians.
         *
         * @param lon_rad            Longitude in radians [-pi, pi].
         * @param lat_rad            Latitude in radians [-pi/2, pi/2].
         * @param elev_m             Elevation in meters (default: 0).
         * @param horizontal_datum   Horizontal reference ellipsoid (default: WGS84).
         *
         * @return Coordinate on success; INVALID_INPUT when components are
         *         non-finite; OUT_OF_BOUNDS when latitude or longitude fall
         *         outside their valid ranges.
         */
        static Result<Geographic> create( double       lon_rad,
                                          double       lat_rad,
                                          double       elev_m           = 0.0,
                                          const Datum& horizontal_datum = Datum::WGS84() );

        /**
         * @brief Factory: build from decimal degrees.
         *
         * @param lat_deg            Latitude in degrees [-90, 90].
         * @param lon_deg            Longitude in degrees [-180, 180].
         * @param elev_m             Elevation in meters (default: 0).
         * @param horizontal_datum   Horizontal reference ellipsoid (default: WGS84).
         */
        static Result<Geographic> from_degrees( double       lat_deg,
                                                double       lon_deg,
                                                double       elev_m           = 0.0,
                                                const Datum& horizontal_datum = Datum::WGS84() );

        /**
         * @brief Factory: build from radians.
         */
        static Result<Geographic> from_radians( double       lat_rad,
                                                double       lon_rad,
                                                double       elev_m           = 0.0,
                                                const Datum& horizontal_datum = Datum::WGS84() );

        /**
         * @brief Get longitude in radians.
         */
        double longitude_radians() const { return m_data[0]; }

        /**
         * @brief Get latitude in radians.
         */
        double latitude_radians() const { return m_data[1]; }

        /**
         * @brief Get elevation in meters.
         */
        double elevation_meters() const { return m_data[2]; }

        /**
         * @brief Get longitude in degrees.
         */
        double longitude_degrees() const { return m_data[0] * RAD_TO_DEG; }

        /**
         * @brief Get latitude in degrees.
         */
        double latitude_degrees() const { return m_data[1] * RAD_TO_DEG; }

        /**
         * @brief Set longitude in degrees.
         */
        void set_longitude_degrees( double lon_deg ) { m_data[0] = lon_deg * DEG_TO_RAD; }

        /**
         * @brief Set latitude in degrees.
         */
        void set_latitude_degrees( double lat_deg ) { m_data[1] = lat_deg * DEG_TO_RAD; }

        /**
         * @brief Set longitude in radians.
         */
        void set_longitude_radians( double lon_rad ) { m_data[0] = lon_rad; }

        /**
         * @brief Set latitude in radians.
         */
        void set_latitude_radians( double lat_rad ) { m_data[1] = lat_rad; }

        /**
         * @brief Set elevation in meters.
         */
        void set_elevation_meters( double elev_m ) { m_data[2] = elev_m; }

        /**
         * @brief Get the horizontal datum.
         */
        const Datum& horizontal_datum() const { return m_horizontal_datum; }

        // Base interface

        /**
         * @brief Get longitude in radians.
         */
        double x() const override { return longitude_radians(); }

        /**
         * @brief Get latitude in radians.
         */
        double y() const override { return latitude_radians(); }

        /**
         * @brief Get elevation in meters.
         */
        double z() const override { return elevation_meters(); }

        /**
         * @brief Geographic coordinates are geodetic, not projected.
         */
        bool is_projected() const override { return false; }

        /**
         * @brief Clone this coordinate.
         */
        std::unique_ptr<Base> clone() const override;

        /**
         * @brief Convert to string.
         */
        std::string to_string( const std::string& format = "pretty" ) const override;

        // Distance

        /**
         * @brief Great-circle distance to another coordinate, in meters.
         *
         * @param other   Target coordinate.
         * @param method  Distance algorithm (default: HAVERSINE).
         */
        Result<double> distance_to( const Geographic& other,
                                    Distance_Method   method = Distance_Method::HAVERSINE ) const;

        /**
         * @brief Great-circle distance between two coordinates, in meters.
         *
         * @param coord1  First coordinate.
         * @param coord2  Second coordinate.
         * @param method  Distance algorithm (default: HAVERSINE).
         *
         * Both coordinates must share the same horizontal datum.  VINCENTY
         * uses the datum's ellipsoid parameters; HAVERSINE uses the datum's
         * semi-major axis as the sphere radius (mean radius for SPHERE()).
         */
        static Result<double> distance( const Geographic& coord1,
                                        const Geographic& coord2,
                                        Distance_Method   method = Distance_Method::HAVERSINE );

    private:

        /**
         * @brief Private constructor — use create(), from_degrees(), or
         *        from_radians() for validated construction.
         */
        Geographic( double       lon_rad,
                    double       lat_rad,
                    double       elev_m,
                    const Datum& horizontal_datum )
            : m_data{ lon_rad, lat_rad, elev_m },
              m_horizontal_datum( horizontal_datum )
        {
        }

        /// Coordinate components: [lon_rad, lat_rad, elev_m].
        std::array<double, 3> m_data{ 0.0, 0.0, 0.0 };

        /// Horizontal reference ellipsoid.
        Datum m_horizontal_datum;

        /**
         * @brief Great-circle distance via the Haversine formula.
         */
        static Result<double> haversine_distance( const Geographic& coord1,
                                                  const Geographic& coord2 );

        /**
         * @brief Distance via Vincenty's inverse formula on the ellipsoid.
         *
         * T. Vincenty, "Direct and Inverse Solutions of Geodesics on the
         * Ellipsoid with Application of Nested Equations", Survey Review
         * 23(176), 1975.
         *
         * @throws std::runtime_error if the iteration fails to converge
         *         (near-antipodal points).
         */
        static Result<double> vincenty_distance( const Geographic& coord1,
                                                 const Geographic& coord2 );

}; // End of Geographic Class

} // end of tmns::geo::coord namespace
