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
 * @file    datum.hpp
 * @author  Marvin Smith
 * @date    10/26/2026
*/
#pragma once

// C++ Standard Libraries
#include <string>
#include <utility>

// Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/outcome.hpp>

namespace tmns::geo {

/**
 * @brief Horizontal reference datum (ellipsoid parameters).
 *
 * Stores the ellipsoid constants needed for geodesy calculations.
 * Additional datums can be added as static factory methods.
 */
class Datum
{
    public:

        /**
         * @brief Create a datum from ellipsoid parameters.
         *
         * @param name                Datum name (e.g. "WGS84").
         * @param semi_major_axis     Equatorial radius in meters.
         * @param inverse_flattening  Inverse flattening (1/f);
         *                            0 implies a sphere.
         *
         * @return Datum on success; INVALID_INPUT for a non-positive
         *         semi-major axis or negative inverse flattening.
         */
        static Result<Datum> create( std::string name,
                                     double      semi_major_axis,
                                     double      inverse_flattening );

        /**
         * @brief WGS-84 reference ellipsoid (EPSG:7030).
         */
        static Datum WGS84();

        /**
         * @brief GRS-80 reference ellipsoid (EPSG:7019).
         */
        static Datum GRS80();

        /**
         * @brief Spherical Earth approximation (mean radius, meters).
         */
        static Datum SPHERE();

        /**
         * @brief Datum name.
         */
        const std::string& name() const
        {
            return m_name;
        }

        /**
         * @brief Set the datum name.
         */
        void set_name( const std::string& name )
        {
            m_name = name;
        }

        /**
         * @brief Semi-major axis (equatorial radius) in meters.
         */
        double semi_major_axis() const
        {
            return m_semi_major_axis;
        }

        /**
         * @brief Set the semi-major axis (equatorial radius) in meters.
         */
        void set_semi_major_axis( double semi_major_axis )
        {
            m_semi_major_axis = semi_major_axis;
        }

        /**
         * @brief Inverse flattening (1/f).  0 implies a sphere.
         */
        double inverse_flattening() const
        {
            return m_inverse_flattening;
        }

        /**
         * @brief Set the inverse flattening (1/f).  0 implies a sphere.
         */
        void set_inverse_flattening( double inverse_flattening )
        {
            m_inverse_flattening = inverse_flattening;
        }

        /**
         * @brief Flattening factor.
         */
        double flattening() const
        {
            return m_inverse_flattening != 0.0 ? 1.0 / m_inverse_flattening : 0.0;
        }

        /**
         * @brief Semi-minor axis (polar radius) in meters.
         */
        double semi_minor_axis() const
        {
            return m_semi_major_axis * ( 1.0 - flattening() );
        }

        /**
         * @brief Compare two datums for equality.
         */
        bool operator==( const Datum& other ) const
        {
            return m_name == other.m_name;
        }

        /**
         * @brief Compare two datums for inequality.
         */
        bool operator!=( const Datum& other ) const
        {
            return !( *this == other );
        }

    private:

        /**
         * @brief Private constructor — use create() or the datum factories.
         *
         * @param name The name of the datum.
         * @param semi_major_axis The semi-major axis (equatorial radius) in meters.
         * @param inverse_flattening The inverse flattening (1/f).
         */
        Datum( std::string name,
               double      semi_major_axis,
               double      inverse_flattening )
            : m_name( std::move( name ) ),
              m_semi_major_axis( semi_major_axis ),
              m_inverse_flattening( inverse_flattening )
        {
        }

        /// @brief Datum name.
        std::string m_name;

        /// @brief Semi-major axis (equatorial radius) in meters.
        double m_semi_major_axis;

        /// @brief Inverse flattening (1/f).
        double m_inverse_flattening;

}; // End of Datum Class

} // end of tmns::geo namespace
