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
 * @file    geo.hpp
 * @author  Marvin Smith
 * @date    10/26/2026
*/
#pragma once

// C++ Standard Libraries
#include <optional>

// Terminus Libraries
#include <terminus/geo/coord/base.hpp>

namespace tmns::geo::coord {

/**
 * @brief Abstract interface for geographic (geodetic) coordinate systems.
 *
 * Any coordinate that can express a latitude/longitude/altitude view of
 * itself implements this, regardless of its internal representation.
 */
class Geo : public Base
{
    public:

        /// @brief Latitude accessor (units defined by the concrete type).
        virtual double latitude() const = 0;

        /// @brief Longitude accessor (units defined by the concrete type).
        virtual double longitude() const = 0;

        /// @brief Altitude accessor, if the coordinate carries one.
        virtual std::optional<double> altitude() const = 0;

}; // End of Geo Class

} // end of tmns::geo::coord namespace
