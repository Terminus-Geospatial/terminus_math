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
 * @file    transform.hpp
 * @author  Marvin Smith
 * @date    11/15/2026
*/
#pragma once

// C++ Standard Libraries
#include <functional>

// Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/geo/crs.hpp>
#include <terminus/math/elevation_policy.hpp>
#include <terminus/math/point.hpp>
#include <terminus/outcome.hpp>

namespace tmns::geo {

/**
 * @brief Point-to-point transform between two CRSs.
 *
 * The interface is `forward`/`inverse` functions — the same contract
 * affine poses, sensor models, and image transforms conform to.
 * `create` resolves a native closed-form path where available;
 * otherwise a backend step is injected by an adapter (PROJ/GDAL via
 * `OGRCoordinateTransformation`) — this library never links a
 * geospatial backend itself.
 *
 * 2D entry points map through the 3D implementation with z handled per
 * `Elevation_Policy` (pass-through by default).
 *
 * @todo In-place bulk overloads over `std::span<PointT>` for image-sized
 *       batches — per-point `Result` doesn't scale to millions of points.
 */
class Transform
{
    public:

        /// One direction of a transform step.
        using Step = std::function<Result<math::Point3d>( const math::Point3d& )>;

        /**
         * @brief Resolve a transform between two CRSs.
         *
         * Identity when source == target.  Natively-known pairs use
         * closed-form math; others fail with NOT_IMPLEMENTED until a
         * backend is injected (see `create_with_backend`).
         */
        static Result<Transform> create( const CRS& source, const CRS& target );

        /**
         * @brief Build a transform around an injected backend.
         *
         * Either step may be empty — a missing direction fails with
         * NOT_SUPPORTED when invoked.
         */
        static Transform create_with_backend(
            const CRS&       source,
            const CRS&       target,
            Step             forward_step,
            Step             inverse_step,
            math::Elevation_Policy policy = math::Elevation_Policy::PASS_THROUGH );

        /**
         * @brief Map a 3D point source -> target.
         */
        Result<math::Point3d> forward( const math::Point3d& p ) const;

        /**
         * @brief Map a 3D point target -> source.
         */
        Result<math::Point3d> inverse( const math::Point3d& p ) const;

        /**
         * @brief 2D convenience — z enters as 0 and exits per the
         *        elevation policy (dropped for PASS_THROUGH).
         */
        Result<math::Point2d> forward( const math::Point2d& p ) const;
        Result<math::Point2d> inverse( const math::Point2d& p ) const;

        /**
         * @brief Whether each direction is callable.
         */
        bool has_forward() const;
        bool has_inverse() const;

        /**
         * @brief Endpoint CRSs.
         */
        const CRS& source() const;
        const CRS& target() const;

        /**
         * @brief z handling.
         */
        math::Elevation_Policy elevation_policy() const;
        void set_elevation_policy( math::Elevation_Policy policy );

    private:

        /// @brief Default constructor.
        Transform() = default;

        /// @brief Source CRS.
        CRS                     m_source;
        /// @brief Target CRS.
        CRS                     m_target;
        /// @brief Forward transformation step.
        Step                    m_forward;
        /// @brief Inverse transformation step.
        Step                    m_inverse;
        /// @brief Z handling policy.
        math::Elevation_Policy  m_elevation_policy = math::Elevation_Policy::PASS_THROUGH;
        bool                    m_identity = false;

}; // End of Transform Class

} // end of tmns::geo namespace
