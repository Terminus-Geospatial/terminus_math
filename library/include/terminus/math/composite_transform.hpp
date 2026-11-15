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
 * @file    composite_transform.hpp
 * @author  Marvin Smith
 * @date    11/15/2026
*/
#pragma once

// C++ Standard Libraries
#include <functional>
#include <vector>

// Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/outcome.hpp>

namespace tmns::math {

/**
 * @brief Ordered chain of point-to-point transform steps.
 *
 * `forward` applies steps left-to-right; `inverse` applies them
 * right-to-left.  Steps are type-erased so a chain can mix affine,
 * CRS, and sensor-model transforms (e.g. pixel -> geo -> geo -> pixel).
 */
template <typename PointT>
class Composite_Transform
{
    public:

        /// One transform step; both directions are optional per step.
        struct Step {
            std::function<Result<PointT>( const PointT& )> forward;
            std::function<Result<PointT>( const PointT& )> inverse;
        };

        /**
         * @brief Append a step to the end of the chain.
         */
        void append( Step step )
        {
            m_steps.push_back( std::move( step ) );
        }

        /**
         * @brief Apply all steps left-to-right.
         *
         * Fails at the first step lacking a forward function or
         * returning an error.
         */
        Result<PointT> forward( const PointT& p ) const
        {
            PointT current = p;
            for( const auto& step : m_steps )
            {
                if( !step.forward )
                {
                    return outcome::fail( error::Error_Code::NOT_SUPPORTED,
                                          "Composite_Transform step has no forward function" );
                }
                auto result = step.forward( current );
                if( result.has_error() )
                {
                    return result;
                }
                current = result.value();
            }
            return outcome::ok<PointT>( current );
        }

        /**
         * @brief Apply all steps right-to-left (inverse order).
         */
        Result<PointT> inverse( const PointT& p ) const
        {
            PointT current = p;
            for( auto it = m_steps.rbegin(); it != m_steps.rend(); ++it )
            {
                if( !it->inverse )
                {
                    return outcome::fail( error::Error_Code::NOT_SUPPORTED,
                                          "Composite_Transform step has no inverse function" );
                }
                auto result = it->inverse( current );
                if( result.has_error() )
                {
                    return result;
                }
                current = result.value();
            }
            return outcome::ok<PointT>( current );
        }

        /**
         * @brief Number of steps in the chain.
         */
        size_t size() const
        {
            return m_steps.size();
        }

        /**
         * @brief True when no steps are registered.
         */
        bool empty() const
        {
            return m_steps.empty();
        }

    private:

        std::vector<Step> m_steps;

}; // End of Composite_Transform Class

} // end of tmns::math namespace
