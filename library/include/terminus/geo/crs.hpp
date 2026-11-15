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
 * @file    crs.hpp
 * @author  Marvin Smith
 * @date    11/15/2026
*/
#pragma once

// C++ Standard Libraries
#include <map>
#include <optional>
#include <string>

// Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/geo/axis.hpp>
#include <terminus/geo/crs_kind.hpp>
#include <terminus/geo/datum.hpp>
#include <terminus/geo/vertical_datum.hpp>
#include <terminus/outcome.hpp>

namespace tmns::geo {

/**
 * @brief Coordinate Reference System identity and classification.
 *
 * `CRS` is an identifier + metadata token — it performs no transform math
 * itself (that is `Transform`).  Design is "hybrid": any CRS can be
 * represented opaquely by authority/WKT, while natively-known systems
 * (WGS-84, UTM, ECEF) carry structured datum/axis/projection data that
 * enables closed-form transforms without a geospatial backend.
 *
 * Axis order is declared explicitly via `Axis_Set`; EPSG:4326 is
 * authority lat/lon but is modeled here as lon/lat (traditional GIS
 * order) unless constructed otherwise.
 */
class CRS
{
    public:

        /**
         * @brief Build from an EPSG code.
         *
         * Recognizes WGS-84 geographic (4326), geocentric (4978), and
         * UTM north/south (326xx/327xx) natively.  Other codes fail with
         * NOT_IMPLEMENTED until an EPSG registry or backend lookup lands.
         */
        static Result<CRS> from_epsg( int code );

        /**
         * @brief Build from a WKT definition.
         *
         * Stored opaquely; classification is best-effort from keywords.
         * Full WKT parsing requires a backend adapter.
         */
        static Result<CRS> from_wkt( std::string wkt );

        /**
         * @brief WGS-84 geographic CRS, lon/lat degrees (EPSG:4326).
         */
        static CRS wgs84();

        /**
         * @brief UTM projected CRS for a zone (EPSG:326xx N / 327xx S).
         */
        static Result<CRS> utm( int zone, bool northern = true );

        /**
         * @brief WGS-84 geocentric CRS, X/Y/Z metres (EPSG:4978).
         */
        static CRS ecef();

        /**
         * @brief Local/engineering frame — for non-geographic poses
         *        such as CV world frames.
         */
        static CRS engineering( std::string name, Axis_Set axes );

        /**
         * @brief CRS classification.
         */
        CRS_Kind kind() const;

        /**
         * @brief Human-readable CRS name.
         */
        const std::string& name() const;

        /**
         * @brief Issuing authority ("EPSG" or empty).
         */
        const std::string& authority() const;

        /**
         * @brief Authority code (EPSG number; 0 when none).
         */
        int code() const;

        /**
         * @brief Kind predicates.
         */
        bool is_geographic() const;
        bool is_projected() const;
        bool is_geocentric() const;
        bool has_vertical() const;

        /**
         * @brief Horizontal datum (ellipsoid), when modeled.
         */
        const std::optional<Datum>& datum() const;

        /**
         * @brief Vertical datum, when modeled.
         */
        const std::optional<Vertical_Datum>& vertical_datum() const;

        /**
         * @brief Declared axes (count, order, units).
         */
        const Axis_Set& axes() const;

        /**
         * @brief Projection method for PROJECTED kinds
         *        (e.g. "transverse_mercator"); empty otherwise.
         */
        const std::string& projection_method() const;

        /**
         * @brief Projection parameters (zone, central_meridian,
         *        false_easting, ...); empty for non-projected kinds.
         */
        const std::map<std::string, double>& projection_params() const;

        /**
         * @brief Stored WKT definition, when the CRS was built from one
         *        or serialized; empty otherwise.
         */
        const std::string& wkt() const;

        /**
         * @brief Identity comparison (authority + code + kind).
         */
        bool operator==( const CRS& other ) const;
        bool operator!=( const CRS& other ) const
        {
            return !( *this == other );
        }

    private:

        /// @brief Kind of CRS.
        CRS_Kind                          m_kind = CRS_Kind::UNSPECIFIED;

        /// @brief Issuing authority ("EPSG" or empty).
        std::string                       m_authority;

        /// @brief Authority code (EPSG number; 0 when none).
        int                               m_code = 0;

        /// @brief Human-readable CRS name.
        std::string                       m_name;

        /// @brief Horizontal datum (ellipsoid), when modeled.
        std::optional<Datum>              m_datum;

        /// @brief Vertical datum, when modeled.
        std::optional<Vertical_Datum>     m_vertical_datum;

        /// @brief Declared axes (count, order, units).
        Axis_Set                          m_axes;

        /// @brief Projection method for PROJECTED kinds (e.g. "transverse_mercator"); empty otherwise.
        std::string                       m_projection_method;

        /// @brief Projection parameters (zone, central_meridian, false_easting, ...); empty for non-projected kinds.
        std::map<std::string, double>     m_projection_params;

        /// @brief Stored WKT definition, when the CRS was built from one or serialized; empty otherwise.
        std::string                       m_wkt;

}; // End of CRS Class

} // end of tmns::geo namespace
