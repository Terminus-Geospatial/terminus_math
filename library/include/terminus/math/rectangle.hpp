/**
 * @file    Rectangle.hpp
 * @author  Marvin Smith
 * @date    7/12/2023
*/
#pragma once

// Terminus Libraries
#include "Point_Utilities.hpp"
#include "Size.hpp"
#include "vector/Vector_Utilities.hpp"

// C++ Libraries
#include <cmath>

namespace tmns::math {

/**
 * Rectangle
*/
template <typename ValueT,
          int      Dims>
class Rectangle
{
    public:

        /**
         * Default constructor
        */
        Rectangle() = default;

        /**
         * Parameterized Constructor
         *
         * @param x
         * @param y
         * @param width
         * @param height
        */
        Rectangle( const ValueT& x,
                   const ValueT& y,
                   const ValueT& width,
                   const ValueT& height )
          : m_bl( { x, y } ),
            m_lengths( { width, 
                         height } )
        {}

        /**
         * Parameterized Constructor
         *
         * @param min_corner
         * @param max_corner
        */
        explicit Rectangle( const Point_<ValueT,Dims>& min_corner,
                            const Point_<ValueT,Dims>& max_corner )
        {
            m_bl = Point_<ValueT,Dims>::elementwise_min( min_corner,
                                                         max_corner );
            
            auto min_point = Point_<ValueT,Dims>::elementwise_min( min_corner,
                                                                   max_corner );
            auto max_point = Point_<ValueT,Dims>::elementwise_max( min_corner,
                                                                   max_corner );

            m_lengths = ( max_point - min_point ).data();
        }

        /**
         * Parameterized Constructor
         *
         * @param bl Bottom-Left Corner
         * @param lengths
        */
        Rectangle( const Point_<ValueT,Dims>& bl,
                   std::array<ValueT,Dims>    lengths )
          : m_bl( bl ),
            m_lengths( lengths )
        {}

        /**
         * Get the dimensions
        */
        size_t dimensions() const
        {
            return Dims;
        }

        /**
         * Get the Width
        */
        ValueT width() const
        {
            return m_lengths.at(0);
        }

        /**
         * Get the Width
        */
        ValueT& width()
        {
            return m_lengths[0];
        }

        /**
         * Get the height
        */
        ValueT height() const
        {
            return m_lengths.at(1);
        }

        /**
         * Get the height
        */
        ValueT& height()
        {
            return m_lengths[1];
        }

        /**
         * Get the depth
        */
        ValueT depth() const
        {
            return m_lengths.at(2);
        }

        /**
         * Get the depth
        */
        ValueT& depth()
        {
            return m_lengths[2];
        }

        /**
         * Get the length
        */
        ValueT length( size_t offset ) const
        {
            return m_lengths.at( offset );
        }

        /**
         * Get the length
        */
        ValueT& length( size_t offset )
        {
            return m_lengths[offset];
        }

        /**
         * Get the bottom-left corner
        */
        Point_<ValueT,Dims> bl() const
        {
            return m_bl;
        }

        /**
         * Get the bottom-left corner reference
        */
        Point_<ValueT,Dims>& bl()
        {
            return m_bl;
        }

        /**
         * Get the top-left corner
        */
        Point_<ValueT,Dims> tl() const
        {
            Point_<ValueT,Dims> offset;
            offset.y() = height();
            return m_bl + offset;
        }

        /**
         * Get the bottom-left corner
        */
        Point_<ValueT,Dims> tr() const
        {
            Point_<ValueT,Dims> offset;
            offset.x() = width();
            offset.y() = height();
            return m_bl + offset;
        }

        /**
         * Get the bottom-left corner
        */
        Point2_<ValueT> br() const
        {
            Point_<ValueT,Dims> offset;
            offset.x() = width();
            return m_bl + offset;
        }

        /**
         * Return point of the minimum range
        */
        Point2_<ValueT> min() const
        {
            return bl();
        }

        /**
         * Return point of the minimum range
        */
        Point2_<ValueT>& min()
        {
            return bl();
        }

        /**
         * Return point of the maximum range
        */
        Point2_<ValueT> max() const
        {
            return tr();
        }

        /**
         * Get the area of the rectangle
        */
        double area() const
        {
            return ( width() * height() );
        }

        /**
         * Get the volume of the rectangle
        */
        double volume() const
        {
            double value = m_lengths[0];
            for( size_t i = 1; i < m_lengths.size(); i++ )
            {
                value *= m_lengths[i];
            }
            return value;
        }

        /**
         * Expand the bounding box by a specified amount
         */
        template <typename ExpansionT>
        Rectangle<ValueT,Dims> expand( const ExpansionT& rate ) const
        {
            auto new_rect = (*this);
            for( size_t i = 0; i < Dims; i++ )
            {
                new_rect.m_bl[i] -= rate;
                new_rect.m_lengths[i] += ( 2 * rate );
            }
            return new_rect;
        }

        /**
         * Recursive component for the subdivision method.
         * 
         * @param bounds
         * @param dim
         * @param current_min
         * @param current_size
         * @param output
        */
        void process_bbox_dims( const std::vector<std::vector<std::tuple<ValueT,ValueT>>>&  bounds,
                                int                                                         dim,
                                Point_<ValueT,Dims>&                                        current_min,
                                Size_<ValueT,Dims>&                                         current_size,
                                std::vector<Rectangle<ValueT,Dims>>&                        output ) const
        {
            if( dim == Dims )
            {
                output.emplace_back( current_min,
                                     current_size.as_vector().data() );
                return;
            }

            // For dimension, add components
            for( size_t dim_idx = 0; dim_idx < bounds[dim].size(); dim_idx++ )
            {
                // Update the current bbox
                current_min[dim]  = std::get<0>( bounds[dim][dim_idx] );
                current_size[dim] = std::get<1>( bounds[dim][dim_idx] );

                process_bbox_dims( bounds,
                                   dim + 1,
                                   current_min,
                                   current_size,
                                   output );
            }
        }

        /**
         * Subdivide the bounding box into chunks
         */
        std::vector<Rectangle<ValueT,Dims>> subdivide( const Size_<ValueT,Dims>& tile_size,
                                                       bool                      include_partials ) const
        {
            Point_<ValueT,Dims> lengths;
            std::vector<ValueT> m_result;

            // Compute the bounds for each dimension  (Dims, Per dim[ start, length])
            std::vector<std::vector<std::tuple<ValueT,ValueT>>> bounds;

            // Iterate over each offset
            for( int dim = 0; dim < Dims; dim++ )
            {
                std::vector<std::tuple<ValueT,ValueT>> dim_indices;

                for( ValueT offset = 0;
                     offset < m_lengths[dim];
                     offset += tile_size[dim] )
                {
                    lengths[dim] = tile_size[dim];
                    if( ( m_lengths[dim] - offset ) < lengths[dim] )
                    {
                        if( !include_partials )
                        {
                            continue;
                        }
                        else
                        {
                            lengths[dim] = m_lengths[dim] - offset;
                        }
                    }

                    dim_indices.push_back( std::make_tuple( min()[dim] + offset,
                                                            lengths[dim] ) );
                }

                bounds.push_back( dim_indices );
            }
            
            Point_<ValueT,Dims> current_min;
            Size_<ValueT,Dims>  current_size;
            std::vector<Rectangle<ValueT,Dims>> output;
            process_bbox_dims( bounds, 0, current_min, current_size, output );

            return output;
        }

        /**
         * Set the max corner of the rectangle
         */
        template<typename PointValueT, int PointDims>
        void set_max( const Point_<PointValueT,PointDims>& new_max )
        {
            // need to compute a new min
            m_bl = Point_<ValueT,Dims>::elementwise_min( bl(), new_max );
            auto max_pt = Point_<ValueT,Dims>::elementwise_max( bl(), new_max );

            auto lengths = ( max_pt - min() ).data();
            auto max_len = std::min( m_lengths.size(), lengths.size() );
            for( size_t x = 0; x < max_len; x++ )
            {
                m_lengths[x] = lengths[x];
            }
        }

        /**
         * Set a new max
        */
        template<typename PointValueT>
        Rectangle<ValueT,Dims> set_max( const Point_<PointValueT,Dims>& new_max ) const
        {
            Rectangle<ValueT,Dims> result;

            // need to compute a new min
            result.m_bl = Point_<ValueT,Dims>::elementwise_min( bl(), new_max );
            auto max_pt = Point_<ValueT,Dims>::elementwise_max( bl(), new_max );

            result.m_lengths = ( max_pt - min() ).data();

            return std::move(result);
        }

        /**
         * Shift the rectangle by the specified amount
         */
        Rectangle<ValueT,Dims> operator + ( const Point_<ValueT,Dims>& offset ) const
        {
            return Rectangle<ValueT,Dims>( bl() + offset,
                                           m_lengths );
        }

        /**
         * Shift the rectangle by the specified amount
         */
        Rectangle<ValueT,Dims> operator + ( const Vector_<ValueT,Dims>& offset ) const
        {
            return Rectangle<ValueT,Dims>( bl() + offset,
                                           m_lengths );
        }

        /**
         * Shift the rectangle by the specified amount
         */
        Rectangle<ValueT,Dims> operator - ( const Point2_<ValueT>& offset ) const
        {
            return Rectangle<ValueT,Dims>( bl() - offset,
                                           m_lengths );
        }

        /**
         * Shift the rectangle by the specified amount
         */
        Rectangle<ValueT,Dims> operator - ( const Vector2_<ValueT>& offset ) const
        {
            return Rectangle<ValueT,Dims>( bl() - offset,
                                           m_lengths );
        }

        /**
         * Check if point inside bounding box.
        */
        template <typename PointValueT>
        bool is_inside( const Point2_<PointValueT>& pt ) const
        {
            return ( pt.x() >= bl().x() &&
                     pt.x() <= tr().x() &&
                     pt.y() >= bl().y() &&
                     pt.y() <= tr().y() );
        }

        /**
         * Check if rectangle is inside bounding box
        */
        template <typename OtherValueT>
        bool is_inside( const Rectangle<OtherValueT,Dims>& obox ) const
        {
            return ( bl().x() <= obox.bl().x() &&
                     bl().y() <= obox.bl().y() &&
                     tr().x() >= obox.tr().x() &&
                     tr().y() >= obox.tr().y() );
        }

        /**
         * Compute the Rectangle Intersection
        */
        template <typename ValueT1, typename ValueT2, int RDims>
        static Rectangle<ValueT1,Dims> intersection( const Rectangle<ValueT1,RDims>& rect1,
                                                     const Rectangle<ValueT2,RDims>& rect2 )
        {
            return Rectangle<ValueT1,RDims>( Point_<ValueT1,RDims>::elementwise_max( rect1.min(),
                                                                                     rect2.min() ),
                                             Point_<ValueT1,RDims>::elementwise_min( rect1.max(),
                                                                                     rect2.max() ) );
        }

        /**
         * Compute the Rectangle Union
        */
        template <typename ValueT1,
                  typename ValueT2,
                  int      RDims>
        static Rectangle<ValueT1,RDims> set_union( const Rectangle<ValueT1,RDims>& rect1,
                                                   const Rectangle<ValueT2,RDims>& rect2 )
        {
            return Rectangle<ValueT1,RDims>( Point_<ValueT1,RDims>::elementwise_min( rect1.min(),
                                                                                     rect2.min() ),
                                             Point_<ValueT1,RDims>::elementwise_max( rect1.max(),
                                                                                     rect2.max() ) );
        }

        /**
         * Compute the Rectangle Union
        */
        template <typename ValueT1, 
                  int      Dims1,
                  typename ValueT2, 
                  int      Dims2>
        static Rectangle<ValueT1,Dims1> set_union( const Rectangle<ValueT1,Dims1>&  rect1,
                                                   const Point_<ValueT2,Dims2>&     point )
        {
            auto point_min = Point_<ValueT2,Dims2>::elementwise_min( rect1.min(),
                                                                     point );
            
            auto point_max = Point_<ValueT2,Dims2>::elementwise_max( rect1.max(),
                                                                     point );

            return Rectangle<ValueT1,Dims1>( point_min,
                                             point_max );
        }

        /**
         * Print to log-friendly string
        */
        std::string to_string() const
        {
            std::stringstream sout;
            sout << "Rectangle<>: bl: " << bl().to_string() << ", Lengths: ";
            size_t counter = 0;
            for( const auto& len : m_lengths )
            {
                if( counter != 0 )
                {
                    sout << ", ";
                }
                sout << len;
                counter++;
            }
            return sout.str();
        }

    private:

        Point_<ValueT,Dims> m_bl;
        std::array<ValueT,Dims> m_lengths;

}; // End of Box class

// Common aliases
using Rect2i = Rectangle<int,2>;
using Rect2f = Rectangle<float,2>;
using Rect2d = Rectangle<double,2>;

} // end of tmns::math