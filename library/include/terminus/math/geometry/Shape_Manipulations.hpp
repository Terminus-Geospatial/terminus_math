/**
 * @file    Shape_Manipulations.hpp
 * @author  Marvin Smith
 * @date    8/11/2023
*/
#pragma once

// Terminus Libraries
#include "../Common_Functions.hpp"
#include "../Size.hpp"
#include "Polygon.hpp"

// C++ Libraries
#include <algorithm>

namespace tmns::math::geom {


/**
 * Construct a Polygon from the definition of a circle
*/
template <typename PointT>
Polygon<PointT> ellipse_to_polygon( const PointT&                center,
                                    const Size_<SizeT,SizeDims>& axis_lengths,
                                    double                       angle_rad,
                                    double                       arc_start_rad,
                                    double                       arc_end_rad,
                                    double                       step_size_rad )
{
    Polygon<PointT> output;

    // Normalize angles
    angle_rad = normalize_angle_radians( angle_rad );

    // Swap arcs if start > end
    if( arc_start_rad > arc_end_rad )
    {
        std::swap( arc_start_rad, arc_end_rad );
    }
    arc_start_rad = normalize_angle_radians( arc_start_rad );
    arc_end_rad   = normalize_angle_radians( arc_end_rad );

    const double TWO_PI = (2 * M_PI);

    if( ( arc_end_rad - arc_start_rad ) > TWO_PI )
    {
        arc_start_rad = 0;
        arc_end_rad   = TWO_PI;
    }

    // Step through the angular range, creating points
    for( double i = arc_start_rad; 
         i < arc_end_rad + step_size_rad; 
         i += step_size_rad )
    {
        // Re-normalize angle
        angle_rad = normalize_angle_radians( i );

        output.append( { center.x() + axes.width()  * std::sin( angle_rad ),
                         center.y() + axes.height() * std::cos( angle_rad ) } );
    }

    return output;
}

} // End of tmns::math::geom namespacep1 - p0