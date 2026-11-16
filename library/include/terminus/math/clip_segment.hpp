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
 * @file    clip_segment.hpp
 * @author  Marvin Smith
 * @date    11/15/2026
*/
#pragma once

// Terminus Libraries
#include <terminus/math/rectangle.hpp>

// C++ Libraries
#include <optional>
#include <utility>

namespace tmns::math {

/**
 * Clip a line segment to an axis-aligned rectangle (Cohen-Sutherland).
 *
 * @param p1     First segment endpoint.
 * @param p2     Second segment endpoint.
 * @param clip   Clipping rectangle; per-axis min/max are resolved from the
 *               rectangle's corners so either corner order is accepted.
 *
 * @returns The clipped endpoint pair when any portion of the segment lies
 *          inside the rectangle, otherwise std::nullopt.  Degenerate
 *          (single-point) segments inside the rectangle are returned as
 *          identical endpoints.
*/
template <typename ValueT>
std::optional<std::pair<Point_<ValueT,2>, Point_<ValueT,2>>>
clip_segment_to_rect( const Point_<ValueT,2>&    p1,
                      const Point_<ValueT,2>&    p2,
                      const Rectangle<ValueT,2>& clip )
{
    enum Out_Code : int { INSIDE = 0, LEFT = 1, RIGHT = 2, BOTTOM = 4, TOP = 8 };

    const auto min_pt = Point_<ValueT,2>::elementwise_min( clip.min(), clip.max() );
    const auto max_pt = Point_<ValueT,2>::elementwise_max( clip.min(), clip.max() );

    const auto out_code = [&min_pt, &max_pt]( const ValueT& x,
                                              const ValueT& y ) -> int
    {
        int code = INSIDE;
        if( x < min_pt.x() ) code |= LEFT;
        if( x > max_pt.x() ) code |= RIGHT;
        if( y < min_pt.y() ) code |= BOTTOM;
        if( y > max_pt.y() ) code |= TOP;
        return code;
    };

    ValueT x0 = p1.x(), y0 = p1.y();
    ValueT x1 = p2.x(), y1 = p2.y();
    int code0 = out_code( x0, y0 );
    int code1 = out_code( x1, y1 );

    while( true )
    {
        if( !( code0 | code1 ) )
        {
            return std::make_pair( Point_<ValueT,2>( { x0, y0 } ),
                                   Point_<ValueT,2>( { x1, y1 } ) );
        }
        if( code0 & code1 )
        {
            return std::nullopt;
        }

        const int clipped = code0 ? code0 : code1;
        ValueT x, y;
        if( clipped & TOP )
        {
            x = x0 + ( x1 - x0 ) * ( max_pt.y() - y0 ) / ( y1 - y0 );
            y = max_pt.y();
        }
        else if( clipped & BOTTOM )
        {
            x = x0 + ( x1 - x0 ) * ( min_pt.y() - y0 ) / ( y1 - y0 );
            y = min_pt.y();
        }
        else if( clipped & RIGHT )
        {
            y = y0 + ( y1 - y0 ) * ( max_pt.x() - x0 ) / ( x1 - x0 );
            x = max_pt.x();
        }
        else
        {
            y = y0 + ( y1 - y0 ) * ( min_pt.x() - x0 ) / ( x1 - x0 );
            x = min_pt.x();
        }

        if( clipped == code0 )
        {
            x0 = x; y0 = y; code0 = out_code( x0, y0 );
        }
        else
        {
            x1 = x; y1 = y; code1 = out_code( x1, y1 );
        }
    }
}

} // end of tmns::math namespace
