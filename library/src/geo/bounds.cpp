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
 * @file    bounds.cpp
 * @author  Marvin Smith
 * @date    11/15/2026
 */

// Terminus Libraries
#include <terminus/geo/bounds.hpp>
#include <terminus/log/logger.hpp>

// C++ Standard Libraries
#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

namespace tmns::geo {

namespace {

/**
 * @brief Accumulate a point into a running geo-space bbox.
 */
void accumulate( const math::Point2d& pt,
                 double&              min_x,
                 double&              min_y,
                 double&              max_x,
                 double&              max_y )
{
    min_x = std::min( min_x, pt.x() );
    min_y = std::min( min_y, pt.y() );
    max_x = std::max( max_x, pt.x() );
    max_y = std::max( max_y, pt.y() );
}

/**
 * @brief The raster's four corners in pixel-edge convention:
 *        (-0.5,-0.5) to (size-0.5, size-0.5).
 */
std::vector<math::Point2d> edge_corners( const math::Size2i& size )
{
    const double max_x = size.width()  - 0.5;
    const double max_y = size.height() - 0.5;
    return
    {
        { { -0.5,  -0.5 } },
        { { max_x, -0.5 } },
        { { max_x, max_y } },
        { { -0.5,  max_y } }
    };
}

/**
 * @brief Densified samples along all four raster edges, pixel-edge
 *        convention.  `samples_per_edge` points on each edge.
 */
std::vector<math::Point2d> densified_edge( const math::Size2i& size,
                                         int                 samples_per_edge )
{
    const double min_x = -0.5;
    const double max_x = size.width()  - 0.5;
    const double min_y = -0.5;
    const double max_y = size.height() - 0.5;

    std::vector<math::Point2d> points;
    points.reserve( static_cast<size_t>( samples_per_edge ) * 4 );

    // Perimeter order: top left->right, right top->bottom, bottom
    // right->left, left bottom->top.  Corner samples repeat at edge
    // joints so each edge keeps the full `samples_per_edge` count.
    for( int i = 0; i < samples_per_edge; ++i )
    {
        const double t = static_cast<double>( i ) / ( samples_per_edge - 1 );
        points.push_back( { { min_x + t * ( max_x - min_x ), min_y } } );
    }
    for( int i = 0; i < samples_per_edge; ++i )
    {
        const double t = static_cast<double>( i ) / ( samples_per_edge - 1 );
        points.push_back( { { max_x, min_y + t * ( max_y - min_y ) } } );
    }
    for( int i = samples_per_edge - 1; i >= 0; --i )
    {
        const double t = static_cast<double>( i ) / ( samples_per_edge - 1 );
        points.push_back( { { min_x + t * ( max_x - min_x ), max_y } } );
    }
    for( int i = samples_per_edge - 1; i >= 0; --i )
    {
        const double t = static_cast<double>( i ) / ( samples_per_edge - 1 );
        points.push_back( { { min_x, min_y + t * ( max_y - min_y ) } } );
    }
    return points;
}

} // end of anonymous namespace


/****************************************/
/*           raster_bounds              */
/****************************************/
Result<math::Rectangle<double,2>> raster_bounds(
    const Georeference_2D& georef,
    const math::Size2i&    raster_size )
{
    if( raster_size.width() <= 0 || raster_size.height() <= 0 )
    {
        return outcome::fail( error::Error_Code::INVALID_INPUT,
                              "raster_bounds: raster size is empty" );
    }

    double min_x =  std::numeric_limits<double>::max();
    double min_y =  std::numeric_limits<double>::max();
    double max_x = std::numeric_limits<double>::lowest();
    double max_y = std::numeric_limits<double>::lowest();

    for( const auto& corner : edge_corners( raster_size ) )
    {
        auto geo_pt = georef.to_geo( corner );
        if( geo_pt.has_error() )
        {
            return geo_pt.error();
        }
        accumulate( geo_pt.value(), min_x, min_y, max_x, max_y );
    }

    return outcome::ok<math::Rectangle<double,2>>(
        math::Rectangle<double,2>( math::Point2d( { min_x, min_y } ),
                                   math::Point2d( { max_x, max_y } ) ) );
}


/****************************************/
/*        transform_edge_points         */
/****************************************/
Result<std::vector<math::Point2d>> transform_edge_points(
    const Georeference_2D&  src_geo,
    const math::Size2i&     src_size,
    const Point_Transform&  src_to_dst,
    int                     edge_samples )
{
    if( src_size.width() <= 0 || src_size.height() <= 0 )
    {
        return outcome::fail( error::Error_Code::INVALID_INPUT,
                              "transform_edge_points: source size is empty" );
    }
    if( edge_samples < 2 )
    {
        return outcome::fail( error::Error_Code::INVALID_INPUT,
                              "transform_edge_points: edge_samples must be >= 2" );
    }

    const auto pixel_pts = densified_edge( src_size, edge_samples );

    std::vector<math::Point2d> dst_pts;
    dst_pts.reserve( pixel_pts.size() );
    for( const auto& pixel_pt : pixel_pts )
    {
        auto src_pt = src_geo.to_geo( pixel_pt );
        if( src_pt.has_error() )
        {
            continue;
        }
        auto dst_pt = src_to_dst( src_pt.value() );
        if( dst_pt.has_error() )
        {
            continue;
        }
        dst_pts.push_back( dst_pt.value() );
    }

    if( dst_pts.empty() )
    {
        return outcome::fail( error::Error_Code::UNKNOWN,
                              "transform_edge_points: no source edge samples "
                              "survived the CRS transform" );
    }
    if( dst_pts.size() < pixel_pts.size() )
    {
        log::warn( "transform_edge_points: dropped ",
                   pixel_pts.size() - dst_pts.size(), " of ",
                   pixel_pts.size(), " edge samples in the CRS transform — "
                   "extent may be under-covered" );
    }
    return outcome::ok<std::vector<math::Point2d>>( std::move( dst_pts ) );
}


/****************************************/
/*          transform_bounds            */
/****************************************/
Result<math::Rectangle<double,2>> transform_bounds(
    const Georeference_2D&  src_geo,
    const math::Size2i&     src_size,
    const Point_Transform&  src_to_dst,
    int                     edge_samples )
{
    auto ring = transform_edge_points( src_geo, src_size,
                                       src_to_dst, edge_samples );
    if( ring.has_error() )
    {
        return ring.error();
    }

    double out_min_x =  std::numeric_limits<double>::max();
    double out_min_y =  std::numeric_limits<double>::max();
    double out_max_x = std::numeric_limits<double>::lowest();
    double out_max_y = std::numeric_limits<double>::lowest();

    for( const auto& dst_pt : ring.value() )
    {
        accumulate( dst_pt, out_min_x, out_min_y, out_max_x, out_max_y );
    }

    return outcome::ok<math::Rectangle<double,2>>(
        math::Rectangle<double,2>( math::Point2d( { out_min_x, out_min_y } ),
                                   math::Point2d( { out_max_x, out_max_y } ) ) );
}


/****************************************/
/*   calculate_default_georeference     */
/****************************************/
Result<Derived_Georeference> calculate_default_georeference(
    const Georeference_2D&  src_geo,
    const math::Size2i&     src_size,
    const CRS&              dst_crs,
    const Point_Transform&  src_to_dst,
    int                     edge_samples )
{
    auto bounds_result = transform_bounds( src_geo, src_size,
                                           src_to_dst, edge_samples );
    if( bounds_result.has_error() )
    {
        return bounds_result.error();
    }
    const auto& bounds = bounds_result.value();

    // Estimate the resolution scale: how much dst geo-space a source
    // pixel covers.  Map the source center pixel and one neighbor on
    // each axis to get a local linear scale.
    const math::Point2d center( { src_size.width()  / 2.0,
                                  src_size.height() / 2.0 } );

    auto to_dst = [&]( const math::Point2d& p ) -> Result<math::Point2d>
    {
        auto src_pt = src_geo.to_geo( p );
        if( src_pt.has_error() )
        {
            return src_pt.error();
        }
        return src_to_dst( src_pt.value() );
    };

    auto c  = to_dst( center );
    auto cx = to_dst( { { center.x() + 1.0, center.y() } } );
    auto cy = to_dst( { { center.x(), center.y() + 1.0 } } );
    if( c.has_error() || cx.has_error() || cy.has_error() )
    {
        return outcome::fail( error::Error_Code::UNKNOWN,
                              "calculate_default_georeference: CRS transform "
                              "failed at the source center" );
    }

    const double res_x = std::abs( cx.value().x() - c.value().x() ) +
                         std::abs( cx.value().y() - c.value().y() );
    const double res_y = std::abs( cy.value().x() - c.value().x() ) +
                         std::abs( cy.value().y() - c.value().y() );
    if( res_x <= 0.0 || res_y <= 0.0 )
    {
        return outcome::fail( error::Error_Code::UNKNOWN,
                              "calculate_default_georeference: zero "
                              "resolution scale in destination CRS" );
    }

    // Output size preserves the source's per-pixel ground scale
    const int out_cols = std::max( 1, static_cast<int>(
        std::lround( bounds.width()  / res_x ) ) );
    const int out_rows = std::max( 1, static_cast<int>(
        std::lround( bounds.height() / res_y ) ) );

    // Build the dst affine: the extent covers pixel edges, and the
    // affine maps pixel centers, so the origin sits a half-pixel
    // inside the bounds.  North-up convention — pixel height negative.
    const double res_out_x = bounds.width()  / out_cols;
    const double res_out_y = bounds.height() / out_rows;
    const double gt[6] =
    {
        bounds.min().x() + 0.5 * res_out_x, res_out_x,  0.0,
        bounds.max().y() - 0.5 * res_out_y, 0.0,       -res_out_y
    };
    auto dst_affine = math::Affine_Transform<2>::from_geotransform( gt );

    Derived_Georeference output
    {
        Georeference_2D( dst_affine, dst_crs ),
        math::Size2i( { out_cols, out_rows } )
    };
    return outcome::ok<Derived_Georeference>( output );
}

} // end of tmns::geo namespace
