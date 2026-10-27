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
 * @file    distance_method_enum.hpp
 * @author  Marvin Smith
 * @date    10/26/2026
*/
#pragma once

namespace tmns::geo {

/**
 * @brief Geodesic distance algorithms supported by coordinate types.
 */
enum class Distance_Method
{
    HAVERSINE,  ///< Spherical great-circle approximation.  Fast, ~0.5% error.
    VINCENTY    ///< Vincenty inverse formula on the ellipsoid.  Accurate to ~0.5 mm.
};

} // end of tmns::geo namespace
