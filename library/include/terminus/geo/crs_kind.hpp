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
 * @file    crs_kind.hpp
 * @author  Marvin Smith
 * @date    11/15/2026
*/
#pragma once

namespace tmns::geo {

/**
 * @brief Classification of a Coordinate Reference System.
 */
enum class CRS_Kind {

    UNSPECIFIED,   ///< No CRS declared.
    GEOGRAPHIC,    ///< Ellipsoidal lon/lat (e.g. WGS-84, EPSG:4326).
    PROJECTED,     ///< Flattened map grid (e.g. UTM).
    GEOCENTRIC,    ///< Earth-centered Cartesian (e.g. ECEF, EPSG:4978).
    VERTICAL,      ///< Height-only reference.
    COMPOUND,      ///< Horizontal + vertical combined.
    ENGINEERING    ///< Local/grid coordinates (covers non-geographic
                   ///< frames such as CV world frames).

}; // End of CRS_Kind enum

} // end of tmns::geo namespace
