/**
 * @file    Point_Transformations.hpp
 * @author  Marvin Smith
 * @dat     10/15/2023
 */
#pragma once

// Terminus Libraries
#include <terminus/coordinate/conversions/vw/Lon_Lat_Rad_To_XYZ_Estimate_Functor.hpp>
#include <terminus/coordinate/conversions/vw/XYZ_to_Lon_Lat_Rad_Estimate_Functor.hpp>

namespace tmns::coordinate::vw {

/**
 * Takes a Vector<ElemT,3> in cartesian 3 space and returns a vector that
 * contains the lat, lon, and radius of that point.  For consistency with
 * cartographic convention, angular values are return in degrees rather 
 * than radians.
 * 
 * Note: The following assumes latitude is measured from the equatorial plane
 * with north positive. This is different than normal spherical coordinate
 * conversion where the equivalent angle is measured from the positive z axis.
 * 
 * Note: notice that the order of the returned triple is longitude, latitude,
 * radius.  This ordering of lon/lat is consistent with the notion of
 * horizontal (x) and vertical (y) coordinates in an image.
 */
template <typename ElementT>
Result<math::Vector_<ElementT,3>> xyz_to_lon_lat_radius_estimate( const math::Vector_<ElementT,3>&  xyz,
                                                                  bool                              east_positive = true,
                                                                  bool                              centered_on_zero = true )
{
    return XYZ_to_Lon_Lat_Rad_Estimate_Functor::apply( xyz,
                                                       east_positive,
                                                       centered_on_zero );
}

/**
 * Takes a Vector<ElemT,3> that contains longitude, latitude, and radius, and
 * a vector that is in cartesian 3-space. For consistency with cartographic
 * convention, angular values are expected to be in degrees rather than radians.
 * 
 * Note: The following assumes latitude is measured from the equatorial plane
 * with north positive. This is different than normal spherical coordinate
 * conversion where the equivalent angle is measured from the positive z axis.
 * 
 * Note: notice that the order of the returned triple is longitude, latitude, radius. 
 * This ordering of lon/lat is consistent with the notion of horizontal (x) and
 * vertical (y) coordinates in an image.
 */
template <typename ElementT>
Result<math::Vector_<ElementT,3>> lon_lat_radius_to_xyz_estimate( const math::Vector_<ElementT,3>& lon_lat_alt,
                                                                  bool                             east_positive = true )
{
    return Lon_Lat_Rad_To_XYZ_Estimate_Functor::apply(lon_lat_alt, east_positive);
}

} // End of tmns::coordinate::vw namespace