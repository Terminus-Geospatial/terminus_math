/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/*                                                                                    */
/*                           Copyright (c) 2024 Terminus LLC                          */
/*                                                                                    */
/*                                All Rights Reserved.                                */
/*                                                                                    */
/*          Use of this source code is governed by LICENSE in the repo root.          */
/*                                                                                    */
/***************************# INTELLECTUAL PROPERTY RIGHTS ****************************/
/**
 * @file    quaternion_utilities.hpp
 * @author  Marvin Smith
 * @date    9/16/2023
 */
#pragma once

// Terminus Libraries
#include <terminus/math/quaternion.hpp>

// C++ Libraries
#include <vector>

namespace tmns::math {

/**
 * Spherical Linear Interpolation
 *
 * @param alpha
 * @param a
 * @param b
 * @param spin
 */
Quaternion slerp( double            alpha,
                  const Quaternion& a,
                  const Quaternion& b,
                  int               spin );

/**
 * Compute the n-weight slerp, analogous to the linear combination
 *  w[0]*Q[0] + ... + w[n-1]*Q[n-1]. This is experimental.
 * We assume the sum of weights is 1.
 */
Quaternion slerp_n( std::vector<double>     w,
                    std::vector<Quaternion> Q,
                    int                     spin );

} // End of tmns::math namespace