/**
 * @file    Polygon.hpp
 * @author  Marvin Smith
 * @date    8/11/2023
*/
#pragma once

namespace tmns::math::geom {

template <typename PointT>
class Polygon
{
    public:

        Polygon() = default;

        /**
         * Get the number of points in the polygon
        */
        size_t size() const
        {
            return m_points.size();
        }

        /**
         * Add a point to the end of the polygon
        */
        void append( PointT pt )
        {
            m_points.push_back( std::move( pt ) );
        }

        /**
         * Close the polygon such that the last point and the first are the "same"
        */
        bool close()
        {
            // Check if already closed, if not, add the first point as the last
            if( size() == 0 )
            {
                return false;
            }
            if( !is_closed() )
            {
                m_points.push_back( m_points[0] );
            }
            return true;
        }

        /**
         * Check if polygon is closed
        */
        bool is_closed() const
        {
            if( size() < 2 )
            {
                return false;
            }
            if( PointT::distance( m_points[0], 
                                  m_points[m_points.size()-1],
                                  DistanceType::L2 ) < 0.001 )
            {
                return true;
            }
            return false;
        }

    private:

        /// List of points
        std::vector<PointT> m_points;

}; // End of Polygon Class

} // End of tmns::math::geom namespace