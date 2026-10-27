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
 * @file    constants.hpp
 * @author  Marvin Smith
 * @date    10/26/2026
*/
#pragma once

// C++ Standard Libraries
#include <cmath>

namespace tmns::geo {

/// @brief Degrees-to-radians conversion factor.
inline constexpr double DEG_TO_RAD = M_PI / 180.0;

/// @brief Radians-to-degrees conversion factor.
inline constexpr double RAD_TO_DEG = 180.0 / M_PI;

} // end of tmns::geo namespace
