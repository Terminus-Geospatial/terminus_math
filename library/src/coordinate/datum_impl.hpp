/**
 * @file    Datum_Impl.hpp
 * @author  Marvin Smith
 * @date    9/20/2023
 */
#pragma once

/// C++ Libraries
#include <sstream>
#include <string>
#include <string_view>

namespace tmns::coordinate::detail {

struct Datum_Impl
{
    /**
     * Set the Semi-Major Axis
     */
    void set_semi_major_axis( double val )
    {
        m_semi_major_axis = val;
        std::ostringstream strm;
        strm << "+a=" << m_semi_major_axis << " +b=" << m_semi_minor_axis;
        if( m_geocentric )
        {
            strm << " +geoc";
        }
        m_proj_str = strm.str();
    }

    /**
     * Set the Semi-Minor Axis
     */
    void set_semi_minor_axis( double val )
    {
        m_semi_minor_axis = val;
        std::ostringstream strm;
        strm << "+a=" << m_semi_major_axis << " +b=" << m_semi_minor_axis;
        if( m_geocentric )
        {
            strm << " +geoc";
        }
        m_proj_str = strm.str();
    }

    /// Name of the Datum
    std::string m_name;

    /// @brief  Spheroid Name
    std::string m_spheroid_name;
        
    /// @brief Meridian Name
    std::string m_meridian_name;
    
    /// @brief Semi Major Axis
    double      m_semi_major_axis;
    
    /// @brief Semi Minor Axis
    double      m_semi_minor_axis;

    /// @brief Meridian Offset
    double      m_meridian_offset;

    /// @brief Flag if Geocentric
    bool        m_geocentric { false };
        
    /// @brief Proj Command String
    std::string m_proj_str;

}; // End of Datum_Impl Namespace


} // End of tmns::coordinate namespace