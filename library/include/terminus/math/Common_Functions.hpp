/**
 * @file    Common_Functions.hpp
 * @author  Marvin Smith
 * @date    8/2/2023
*/
#pragma once

// C++ Libraries
#include <cmath>

// Contains overrides of common math operations

namespace tmns::math {

/**
 * Re-align an angle to the range of [0,360]
*/
template <typename ValueT>
ValueT normalize_angle_degrees( ValueT angle )
{
    while( angle < 0 )
    {
        angle += 360;
    }
    while( angle > 360 )
    {
        angle -= 360;
    }
    return angle;
}


/**
 * Re-align an angle to the range of [0,2-pi]
*/
template <typename ValueT>
ValueT normalize_angle_radians( ValueT angle )
{
    while( angle < 0 )
    {
        angle += (2 * M_PI);
    }
    while( angle > (2 * M_PI) )
    {
        angle -= (2 * M_PI);
    }
    return angle;
}




} // End of tmns::math namespace