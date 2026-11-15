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
 * @file    axis.cpp
 * @author  Marvin Smith
 * @date    11/15/2026
*/

// Terminus Libraries
#include <terminus/geo/axis.hpp>

namespace tmns::geo {


/****************************************/
/*          lon_lat_degrees             */
/****************************************/
Axis_Set Axis_Set::lon_lat_degrees()
{
    return Axis_Set( { { "lon", Axis_Direction::EAST,  "degree" },
                       { "lat", Axis_Direction::NORTH, "degree" } } );
}


/****************************************/
/*          lat_lon_degrees             */
/****************************************/
Axis_Set Axis_Set::lat_lon_degrees()
{
    return Axis_Set( { { "lat", Axis_Direction::NORTH, "degree" },
                       { "lon", Axis_Direction::EAST,  "degree" } } );
}


/****************************************/
/*     easting_northing_meters          */
/****************************************/
Axis_Set Axis_Set::easting_northing_meters()
{
    return Axis_Set( { { "easting",  Axis_Direction::EAST,  "metre" },
                       { "northing", Axis_Direction::NORTH, "metre" } } );
}


/****************************************/
/*           ecef_meters                */
/****************************************/
Axis_Set Axis_Set::ecef_meters()
{
    return Axis_Set( { { "x", Axis_Direction::OTHER, "metre" },
                       { "y", Axis_Direction::OTHER, "metre" },
                       { "z", Axis_Direction::UP,    "metre" } } );
}


/****************************************/
/*            Axis_Set                  */
/****************************************/
Axis_Set::Axis_Set( std::vector<Axis> axes )
    : m_axes( std::move( axes ) )
{
}


/****************************************/
/*              size                    */
/****************************************/
size_t Axis_Set::size() const
{
    return m_axes.size();
}


/****************************************/
/*            operator[]                */
/****************************************/
const Axis& Axis_Set::operator[]( size_t index ) const
{
    return m_axes[index];
}


/****************************************/
/*      is_traditional_gis_order        */
/****************************************/
bool Axis_Set::is_traditional_gis_order() const
{
    return !m_axes.empty() && m_axes.front().direction == Axis_Direction::EAST;
}


} // end of tmns::geo namespace
