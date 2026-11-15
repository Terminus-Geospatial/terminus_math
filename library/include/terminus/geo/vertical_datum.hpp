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
 * @file    vertical_datum.hpp
 * @author  Marvin Smith
 * @date    11/15/2026
*/
#pragma once

// C++ Standard Libraries
#include <string>

namespace tmns::geo {

/**
 * @brief Height reference surface — companion to the (horizontal) `Datum`.
 *
 * Required for 3D and compound CRSs once elevation-aware transforms land;
 * most operations today keep elevation as a pass-through.
 */
class Vertical_Datum
{
    public:

        /// Reference surface kind.
        enum class Kind {
            UNSPECIFIED,
            ELLIPSOIDAL,   ///< Height above the ellipsoid (native to the
                           ///< horizontal datum's CRS; no geoid needed).
            ORTHOMETRIC,   ///< Height above a geoid model
                           ///< (EGM96/EGM2008, NAVD88, ...).
            GEOID          ///< Geoid undulation surface itself.
        };

        /**
         * @brief Ellipsoidal height (no separate surface).
         */
        static Vertical_Datum ellipsoidal()
        {
            return Vertical_Datum( Kind::ELLIPSOIDAL, "Ellipsoidal" );
        }

        /**
         * @brief EGM96 geoid (EPSG:5773 heights).
         */
        static Vertical_Datum egm96()
        {
            return Vertical_Datum( Kind::GEOID, "EGM96" );
        }

        /**
         * @brief EGM2008 geoid (EPSG:3855 heights).
         */
        static Vertical_Datum egm2008()
        {
            return Vertical_Datum( Kind::GEOID, "EGM2008" );
        }

        /**
         * @brief Generic construction.
         */
        Vertical_Datum( Kind kind, std::string name )
            : m_kind( kind ),
              m_name( std::move( name ) )
        {
        }

        /**
         * @brief Surface kind.
         */
        Kind kind() const
        {
            return m_kind;
        }

        /**
         * @brief Datum name.
         */
        const std::string& name() const
        {
            return m_name;
        }

    private:

        Kind        m_kind = Kind::UNSPECIFIED;
        std::string m_name;

}; // End of Vertical_Datum Class

} // end of tmns::geo namespace
