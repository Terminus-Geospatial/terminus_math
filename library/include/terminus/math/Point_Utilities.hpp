/**
 * @file    Point_Utilities.hpp
 * @author  Marvin Smith
 * @date    7/14/2023
*/
#pragma once

// Terminus Libraries
#include "Point.hpp"

namespace tmns::math {

/**
 * Convert 2 scalars to 3D point
 */
template <typename ValueT>
Point2_<ValueT> ToPoint2( const ValueT& x,
                          const ValueT& y )
{
    return {{ x, y }};
}

/**
 * Convert 3 scalars to 3D point
 */
template <typename ValueT>
Point3_<ValueT> ToPoint3( const ValueT& x,
                          const ValueT& y,
                          const ValueT& z )
{
    return {{ x, y, z }};
}

} // End of tmns::math namespace