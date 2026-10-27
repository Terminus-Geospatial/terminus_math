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
 * @file    utm.hpp
 * @author  Marvin Smith
 * @date    10/26/2026
*/
#pragma once

// C++ Standard Libraries
#include <array>
#include <cstdint>
#include <memory>
#include <string>

// Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/outcome.hpp>
#include <terminus/geo/constants.hpp>
#include <terminus/geo/coord/base.hpp>
#include <terminus/geo/datum.hpp>

namespace tmns::geo::coord {

/**
 * @brief UTM hemisphere.
 */
enum class Hemisphere
{
    NORTH,
    SOUTH
};

/**
 * @brief Universal Transverse Mercator (UTM) coordinate.
 *
 * A transverse-Mercator projected coordinate expressed as easting/northing
 * meters within one of 60 zones plus a hemisphere.  Zone 1 covers
 * [-180, -174) degrees longitude, each zone 6 degrees wide.
 */
class UTM : public Base
{
    public:

        /// @brief Valid UTM zone range.
        static constexpr std::int32_t MIN_ZONE = 1;
        static constexpr std::int32_t MAX_ZONE = 60;

        /// @brief Valid easting range in meters.
        static constexpr double MIN_EASTING  = 100000.0;
        static constexpr double MAX_EASTING  = 900000.0;

        /// @brief Valid northing range in meters.
        static constexpr double MIN_NORTHING = 0.0;
        static constexpr double MAX_NORTHING = 10000000.0;

        /**
         * @brief Create a UTM coordinate.
         *
         * @param zone               UTM zone [1, 60].
         * @param hemisphere         NORTH or SOUTH.
         * @param easting_m          Easting in meters [100000, 900000].
         * @param northing_m         Northing in meters [0, 10000000].
         * @param elev_m             Elevation in meters (default: 0).
         * @param horizontal_datum   Reference ellipsoid (default: WGS84).
         *
         * @return Coordinate on success; INVALID_INPUT for non-finite
         *         components; OUT_OF_BOUNDS when zone, easting, or northing
         *         fall outside their valid ranges.
         */
        static Result<UTM> create( std::int32_t zone,
                                   Hemisphere   hemisphere,
                                   double       easting_m,
                                   double       northing_m,
                                   double       elev_m           = 0.0,
                                   const Datum& horizontal_datum = Datum::WGS84() );

        /// @brief UTM zone [1, 60].
        std::int32_t zone() const { return m_zone; }

        /// @brief Hemisphere.
        Hemisphere hemisphere() const { return m_hemisphere; }

        /// @brief Easting in meters.
        double easting_meters() const { return m_data[0]; }

        /// @brief Northing in meters.
        double northing_meters() const { return m_data[1]; }

        /// @brief Elevation in meters.
        double elevation_meters() const { return m_data[2]; }

        void set_easting_meters(  double easting_m  ) { m_data[0] = easting_m; }
        void set_northing_meters( double northing_m ) { m_data[1] = northing_m; }
        void set_elevation_meters( double elev_m    ) { m_data[2] = elev_m; }

        /// @brief Reference ellipsoid.
        const Datum& horizontal_datum() const { return m_horizontal_datum; }

        /**
         * @brief Compute the UTM zone for a longitude in radians.
         *
         * Zone 1 covers [-180, -174); zone 60 covers [174, 180].
         */
        static std::int32_t zone_for_longitude( double lon_rad );

        /**
         * @brief Central meridian of a zone in radians.
         */
        static double central_meridian( std::int32_t zone );

        // Base interface

        double x() const override { return easting_meters(); }
        double y() const override { return northing_meters(); }
        double z() const override { return elevation_meters(); }

        /**
         * @brief UTM is a projected coordinate system — XY distances are
         *        Euclidean in meters.
         */
        bool is_projected() const override { return true; }

        /**
         * @brief Clone this coordinate.
         */
        std::unique_ptr<Base> clone() const override;

        /**
         * @brief Convert this coordinate to a string.
         */
        std::string to_string( const std::string& format = "pretty" ) const override;

        /**
         * @brief Euclidean distance to another UTM coordinate, in meters.
         *
         * Only valid within the same zone, hemisphere, and datum.
         */
        Result<double> distance_to( const UTM& other ) const;

        /**
         * @brief Euclidean distance between two UTM coordinates, in meters.
         */
        static Result<double> distance( const UTM& coord1,
                                        const UTM& coord2 );

    private:

        /**
         * @brief Private constructor — use create() for validated
         *        construction.
         */
        UTM( std::int32_t zone,
             Hemisphere   hemisphere,
             double       easting_m,
             double       northing_m,
             double       elev_m,
             const Datum& horizontal_datum )
            : m_data{ easting_m, northing_m, elev_m },
              m_zone( zone ),
              m_hemisphere( hemisphere ),
              m_horizontal_datum( horizontal_datum )
        {
        }

        /// Coordinate components: [easting, northing, elevation] in meters.
        std::array<double, 3> m_data{ 0.0, 0.0, 0.0 };

        /// UTM zone.
        std::int32_t m_zone = 0;

        /// Hemisphere.
        Hemisphere m_hemisphere = Hemisphere::NORTH;

        /// Reference ellipsoid.
        Datum m_horizontal_datum;

}; // End of UTM Class

} // end of tmns::geo::coord namespace
