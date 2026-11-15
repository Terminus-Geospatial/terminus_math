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
 * @file    sensor_model.hpp
 * @author  Marvin Smith
 * @date    11/15/2026
*/
#pragma once

// Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/geo/crs.hpp>
#include <terminus/math/point.hpp>
#include <terminus/outcome.hpp>

namespace tmns::geo {

/**
 * @brief Interface for parametric sensor models (future).
 *
 * A pose (`Georeference`) is the simplest sensor model: position +
 * orientation mapping image coordinates to object space.  RPCs
 * (NITF, pushbroom) are the generalized variant — same role, richer
 * parameterization, and the same forward/inverse contract.
 *
 * `to_object` maps image coordinates (typically line/sample) into
 * object space (the model's CRS, usually 3D geographic/ECEF).
 * `to_image` is optional — iterative solvers may only offer the
 * forward direction.
 */
template <int ImageDim = 2, int GeoDim = 3>
class Sensor_Model
{
    public:

        /// Image-space point (line/sample).
        using Image_PointT = math::Point_<double, ImageDim>;

        /// Object-space point (geo/ECEF).
        using Geo_PointT = math::Point_<double, GeoDim>;

        virtual ~Sensor_Model() = default;

        /**
         * @brief Image -> object space (ground intersection).
         */
        virtual Result<Geo_PointT> to_object( const Image_PointT& image ) const = 0;

        /**
         * @brief Object -> image space; NOT_SUPPORTED by default.
         */
        virtual Result<Image_PointT> to_image( const Geo_PointT& geo ) const
        {
            (void)geo;
            return outcome::fail( error::Error_Code::NOT_SUPPORTED,
                                  "Sensor_Model provides no image-direction solution" );
        }

        /**
         * @brief Object-space CRS the model resolves into.
         */
        virtual const CRS& object_crs() const = 0;

}; // End of Sensor_Model Class

} // end of tmns::geo namespace
