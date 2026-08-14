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
 * @file    example.cpp
 * @author  Marvin Smith
 * @date    8/14/2026
 */

// Terminus Libraries
#include <terminus/math/exports.hpp>
#include <terminus/math/rectangle.hpp>

// C++ Standard Libraries
#include <iostream>
#include <sstream>

int main()
{
    // 1 - Build a basic rectangle and exercise a few accessors.
    tmns::math::Rect2i rect( /* x      = */ 10,
                             /* y      = */ 20,
                             /* width  = */ 640,
                             /* height = */ 480 );

    std::cout << "Rectangle: " << rect.to_string() << std::endl;
    std::cout << "  width:  " << rect.width()  << std::endl;
    std::cout << "  height: " << rect.height() << std::endl;
    std::cout << "  area:   " << rect.area()   << std::endl;

    if( rect.width() != 640 || rect.height() != 480 )
    {
        std::cerr << "Rectangle did not report the expected dimensions." << std::endl;
        return 1;
    }

    // 2 - Get build information and log this
    auto build_info = tmns::math::get_build_info();
    std::stringstream ss;
    ss << "Terminus Math Build Information:\n";
    for( const auto& [key, value] : build_info )
    {
        ss << key << ": " << value << "\n";
    }
    std::cout << ss.str() << std::endl;

    return 0;
}
