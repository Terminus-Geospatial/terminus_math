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
 * @file    elevation_policy.hpp
 * @author  Marvin Smith
 * @date    11/15/2026
*/
#pragma once

namespace tmns::math {

/**
 * @brief How a transform treats the elevation (z) component.
 *
 * Most transforms are horizontal-only; the declared policy tells the
 * caller what happens to z when a 3D point passes through.
 */
enum class Elevation_Policy {

    /// z is copied through untouched (default; horizontal-only transforms).
    PASS_THROUGH,

    /// The transform consumes z as an input (e.g. terrain-referenced
    /// projections that read height but do not produce a new one).
    CONSUME,

    /// The transform produces a new z (e.g. datum/geoid height shifts).
    PRODUCE

}; // End of Elevation_Policy enum

} // end of tmns::math namespace
