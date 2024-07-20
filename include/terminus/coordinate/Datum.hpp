/**
 * @file    Datum.hpp
 * @author  Marvin Smith
 * @date    9/20/2023
 */
#pragma once

// C++ Libraries
#include <memory>
#include <string_view>

// Terminus Libraries
#include <terminus/core/error/ErrorCategory.hpp>
#include <terminus/math/matrix/Matrix.hpp>
#include <terminus/math/vector/Vector.hpp>

// In case GDAL isn't specified
class OGRSpatialReference;

namespace tmns::coordinate {

/**
 * Forward declare the underlying datum impl
 */
namespace detail {

struct Datum_Impl;

} // End of detail namespace

/**
 * A geodetic datum, i.e. a reference ellipsoid coordinate system for a planetary
 * body.  This implementation assumes a relatively modern notion of a datum, ie. a
 * geocentric bi-axial ellipsoidal model. 
 * 
 * To express a spherical datum, set the semi-major axis equal to the semi-minor axis.
 * All angles are measured in degrees, and all distances are measured in meters.  This
 * class incorporates a prime meridian offset, which is not usually strictly considered
 * part of the datum but which has no better place to be right now.
 */
class Datum
{
    public:

        /**
         * Default Constructor:  Creates WGS84 Datum
         */
        Datum();

        /**
         * Constructor for custom datum types
         * 
         * @param name
         * @param spheroid_name
         * @param meridian_name
         * @param semi_major_axis
         * @param semi_minor_axis
         * @param meridian_offset
         */
        Datum( std::string_view name,
               std::string_view spheroid_name,
               std::string_view meridian_name,
               double           semi_major_axis,
               double           semi_minor_axis,
               double           meridian_offset );
        
        /**
         * Get the name of the datum.
         */
        const std::string& name() const;

        /**
         * Spheroid Name (const)
         */
        const std::string& spheroid_name() const;

        /**
         * Meridian Name
         */
        const std::string& meridian_name() const;

        /**
         * Semi Major Axis
         */
        const double& semi_major_axis() const;

        /**
         * Semi Minor Axis
         */
        const double& semi_minor_axis() const;

        /**
         * Meridian Offset
         */
        const double& meridian_offset() const;

        /**
         * Check if Geocentric
         */
        bool geocentric() const;

        /**
         * Returns the radius (distance from center of the body) at the given lat/lon
         */
        double radius( double lon_deg,
                       double lat_deg ) const;

        /**
         * Return geocentric latitude corresponding to geodetic lat:
         * - Geocentric latitude is measured with body's center rather than tangent to surface.
         * - For spherical datums these values are identical.
         */
        //double geocentric_latitude(double lat) const;

        /**
         * Return radius of curvature in the prime vertical.
         */
        //double radius_of_curvature(double lon, double lat) const;

        /**
         * Return distance from the center of the Earth (lat,lon are geodetic,
         * alt is ellipsoidal or geodetic height).
         */
        //double geocentric_radius(double lon, double lat, double alt = 0.0) const;

        /**
         * Inverse Flattening Coefficient (1/a)
         */
        //double inverse_flattening() const;

        /**
         * Return cartesian (ECEF) coordinates of geodetic coordinates p [Lon, Lat, Height]
         */
        math::Vector3d geodetic_to_cartesian( const math::Vector3d& llh ) const;

        /**
         * Return the rotation matrix for converting between ECEF and NED vectors. If v 
         * is a Cartesian (ECEF) vector, the inverse of this matrix times v will find 
         * v's components in the North, East, and Down directions at given lon and lat.
         * And the reverse holds, if v is in the NED coordinate system, this matrix times
         * v will be its expression in ECEF.
         * This assumes the planet to be a perfect sphere, which can be 
         * problematic.
         */
        math::Matrix_3x3 lonlat_to_ned_matrix( const math::Vector2d& lonlat) const;

        /**
         * Convert cartesian XYZ coordinates into latitude/longitude/altitude
         */
        math::Vector3d cartesian_to_geodetic( const math::Vector3d& xyz ) const;
         
        /**
         * Create a Datum using a well-known name
         * - Supported:  WGS84, WGS72, NAD83, NAD27, D_MOON, D_MARS, MOLA.
         * 
         * @note:  If you do not have ISIS support, Moon, Mars, and Mola will likely be disabled.
         */
        static Result<Datum> from_well_known_name( const std::string& name );

        /**
         * Set Datum from OGR Spatial Ref
         */
        static Result<Datum> from_ogr_spatial_ref( const OGRSpatialReference& ref );

        /**
         * Set Datum from Proj String
         */
        static Result<Datum> from_proj_string( const std::string& proj_str );

        /**
         * Set Datum from WKT string.
         */
        static Result<Datum> from_well_known_text( const std::string& wkt_string );

    private:

        /// @brief Underlying Datum Type
        std::shared_ptr<detail::Datum_Impl> m_impl;

}; // End of Datum class

} // End of tmns::coordinate namespace