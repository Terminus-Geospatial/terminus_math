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
 * @file    georeference.hpp
 * @author  Marvin Smith
 * @date    11/15/2026
*/
#pragma once

// Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/geo/crs.hpp>
#include <terminus/math/affine_transform.hpp>
#include <terminus/math/point.hpp>
#include <terminus/outcome.hpp>

namespace tmns::geo {

/**
 * @brief A raster's pose: affine transform + CRS in one object.
 *
 * The affine maps pixel coordinates into the CRS frame (`to_geo`);
 * its inverse maps geo back to pixel (`to_pixel`).  Pairing multiple
 * `Georeference`s with a `Transform` between their CRSs yields the
 * full pixel -> pixel chain used by image reprojection.
 */
template <int Dim = 2>
class Georeference
{
    public:

        /// Point type for this dimension.
        using PointT = math::Point_<double, Dim>;

        /**
         * @brief Construct from pose components.
         */
        Georeference( math::Affine_Transform<Dim> affine, CRS crs )
            : m_affine( affine ),
              m_crs( std::move( crs ) )
        {
        }

        /**
         * @brief Build from a GDAL geotransform + CRS (2D only).
         */
        static Georeference<2> from_geotransform( const double gt[6], CRS crs )
        {
            return Georeference<2>(
                math::Affine_Transform<2>::from_geotransform( gt ),
                std::move( crs ) );
        }

        /**
         * @brief Map pixel -> geo coordinates.
         */
        Result<PointT> to_geo( const PointT& pixel ) const
        {
            return outcome::ok<PointT>( m_affine.forward( pixel ) );
        }

        /**
         * @brief Map geo -> pixel coordinates.
         *
         * Fails when the affine is singular.
         */
        Result<PointT> to_pixel( const PointT& geo ) const
        {
            return m_affine.inverse( geo );
        }

        /**
         * @brief Pixel <-> geo pose.
         */
        const math::Affine_Transform<Dim>& affine() const
        {
            return m_affine;
        }

        /**
         * @brief Coordinate system the pose maps into.
         */
        const CRS& crs() const
        {
            return m_crs;
        }

    private:

        math::Affine_Transform<Dim> m_affine = math::Affine_Transform<Dim>::identity();
        CRS                         m_crs;

}; // End of Georeference Class

/// Common aliases.
using Georeference_2D = Georeference<2>;
using Georeference_3D = Georeference<3>;

} // end of tmns::geo namespace
