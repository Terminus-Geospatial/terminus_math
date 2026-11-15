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
 * @file    conversions.hpp
 * @author  Marvin Smith
 * @date    11/15/2026
*/
#pragma once

// Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/geo/coord/ecef.hpp>
#include <terminus/geo/coord/geographic.hpp>
#include <terminus/geo/coord/utm.hpp>
#include <terminus/outcome.hpp>

namespace tmns::geo::coord {

/**
 * @brief Closed-form conversions between the native coordinate types.
 *
 * These cover the analytically-solvable pairs (geodetic <-> geocentric,
 * geodetic <-> UTM Transverse Mercator).  `Transform::create` dispatches
 * to them when both endpoint CRSs are natively modeled; everything else
 * needs a backend adapter.
 */

/**
 * @brief Geodetic (lon/lat/h) -> geocentric ECEF (x/y/z).
 */
Result<ECEF> to_ecef( const Geographic& geographic );

/**
 * @brief Geocentric ECEF (x/y/z) -> geodetic (lon/lat/h).
 *
 * Iterative solution; converges to sub-millimeter height in a few steps.
 * The result datum matches the ECEF coordinate's datum.
 */
Result<Geographic> to_geographic( const ECEF& ecef );

/**
 * @brief Geodetic -> UTM.  Zone is auto-selected from longitude when
 *        `zone == 0`.  Elevation passes through unchanged.
 */
Result<UTM> to_utm( const Geographic& geographic, std::int32_t zone = 0 );

/**
 * @brief Geodetic -> UTM with an explicit hemisphere.
 *
 * Uses the declared hemisphere's false northing rather than deriving it
 * from the latitude sign.  This keeps transforms into a named UTM CRS
 * (e.g. EPSG:32631, north) continuous across the equator — a point at
 * -0.001 degrees yields a small negative northing instead of jumping to
 * ~10M, matching PROJ behavior.
 */
Result<UTM> to_utm( const Geographic& geographic,
                    std::int32_t      zone,
                    Hemisphere        hemisphere );

/**
 * @brief UTM -> geodetic.  Elevation passes through unchanged.
 */
Result<Geographic> to_geographic( const UTM& utm );

} // end of tmns::geo::coord namespace
