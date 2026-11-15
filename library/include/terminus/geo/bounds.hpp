/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/*                                                                                    */
/*                           Copyright (c) 2026 Terminus LLC                          */
/*                                                                                    */
/*                                All Rights Reserved.                                */
/*                                                                                    */
/*          Use of this source code is governed by LICENSE in the repo root.          */
/*                                                                                    */
/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/**
 * @file    bounds.hpp
 * @author  Marvin Smith
 * @date    11/15/2026
 */
#pragma once

// C++ Standard Libraries
#include <functional>
#include <vector>

// Terminus Libraries
#include <terminus/geo/georeference.hpp>
#include <terminus/math/point.hpp>
#include <terminus/math/rectangle.hpp>
#include <terminus/math/size.hpp>
#include <terminus/outcome.hpp>

namespace tmns::geo {

/**
 * @brief Callable mapping a geo-space point between CRS frames.
 *
 * Typically a `Transform::forward`/`inverse` bound to a direction, or an
 * injected GDAL/PROJ backend.  Used by the bounds functions instead of a
 * concrete `Transform` so callers control direction and backend.
 */
using Point_Transform =
    std::function<Result<math::Point2d>( const math::Point2d& )>;

/**
 * @brief A destination georeference derived for a target CRS.
 *
 * Result of `calculate_default_georeference` — carries the recommended
 * raster size alongside the pose since the caller needs both to allocate
 * an output image.
 */
struct Derived_Georeference
{
    Georeference_2D georef;
    math::Size2i    size;

}; // End of Derived_Georeference Struct

/**
 * @brief Geo-space bounds of a raster in its native CRS.
 *
 * Pushes the four pixel-edge corners through the georeference's affine
 * and takes the bbox.  Exact for affine poses (including rotation);
 * does NOT account for CRS reprojection — use `transform_bounds` to get
 * the extent in another CRS.
 */
Result<math::Rectangle<double,2>> raster_bounds(
    const Georeference_2D& georef,
    const math::Size2i&    raster_size );

/**
 * @brief Densified raster-edge samples transformed through a CRS map —
 * the reprojected footprint polygon.
 *
 * Samples `edge_samples` points per raster edge (pixel-edge convention:
 * the outer boundary runs (-0.5,-0.5) to (size-0.5, size-0.5)), maps each
 * through `src_geo.to_geo` then `src_to_dst`, and returns the ring in
 * destination-CRS coordinates, ordered around the perimeter.
 *
 * This is the primitive `transform_bounds` bboxes over.  Callers that
 * need the actual footprint shape — projected footprints for catalogs,
 * seam lines, or spatial intersection tests against a curved raster
 * boundary — use this directly.
 *
 * Samples that fail either transform step are dropped; if every sample
 * fails the call returns an error.
 *
 * @param src_geo      Source pose
 * @param src_size     Source raster dimensions
 * @param src_to_dst   CRS map (geo space, source -> destination)
 * @param edge_samples Points sampled per edge (>= 2)
 */
Result<std::vector<math::Point2d>> transform_edge_points(
    const Georeference_2D&  src_geo,
    const math::Size2i&     src_size,
    const Point_Transform&  src_to_dst,
    int                     edge_samples = 21 );

/**
 * @brief Geo-space bounds of a raster transformed through a CRS map.
 *
 * Source pixel-edge bounds are densified (`edge_samples` points per
 * edge) before transformation so curved reprojection edges are not
 * clipped — sampling only the four corners under-covers the true
 * extent when edges bulge in the destination CRS.
 *
 * @param src_geo      Source pose
 * @param src_size     Source raster dimensions
 * @param src_to_dst   CRS map (geo space, source -> destination)
 * @param edge_samples Points sampled per edge (>= 2)
 */
Result<math::Rectangle<double,2>> transform_bounds(
    const Georeference_2D&  src_geo,
    const math::Size2i&     src_size,
    const Point_Transform&  src_to_dst,
    int                     edge_samples = 21 );

/**
 * @brief Derive a destination georeference when only the CRS is named —
 * the `rasterio.warp.calculate_default_transform` analog.
 *
 * Computes the densified-edge extent in the destination CRS, then picks
 * a pixel size that preserves the source's native resolution scale.
 *
 * @param src_geo      Source pose
 * @param src_size     Source raster dimensions
 * @param dst_crs      Destination CRS
 * @param src_to_dst   CRS map (geo space, source -> destination)
 * @param edge_samples Densification count passed to `transform_bounds`
 */
Result<Derived_Georeference> calculate_default_georeference(
    const Georeference_2D&  src_geo,
    const math::Size2i&     src_size,
    const CRS&              dst_crs,
    const Point_Transform&  src_to_dst,
    int                     edge_samples = 21 );

} // end of tmns::geo namespace
