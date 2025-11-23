/**
 * @file    Poly_Line.hpp
 * @author  Marvin Smith
 * @date    8/11/2023
*/
#pragma once

namespace tmns::math {

/**
 * @class Poly_Line
*/
template <typename PointT>
class Poly_Line
{
    public:

        /**
         * Default Constructor
        */
        Poly_Line() = default;

    private:

        /// Underlying Boost Geometry Object
        std::vector<PointT> m_polyline;

}; // End of Poly_Line Class

} // End of tmns::math namespace