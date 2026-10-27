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
 * @file    datum.cpp
 * @author  Marvin Smith
 * @date    10/26/2026
*/

// Terminus Libraries
#include <terminus/geo/datum.hpp>

namespace tmns::geo {

/****************************************/
/*             Create Datum             */
/****************************************/
Result<Datum> Datum::create( std::string name,
                             double      semi_major_axis,
                             double      inverse_flattening )
{
    if( semi_major_axis <= 0.0 )
    {
        return outcome::fail( error::Error_Code::INVALID_INPUT,
                                "Datum requires a positive semi-major axis" );
    }
    if( inverse_flattening < 0.0 )
    {
        return outcome::fail( error::Error_Code::INVALID_INPUT,
                                "Datum requires a non-negative inverse flattening" );
    }
    return outcome::ok<Datum>(
        Datum( std::move( name ), semi_major_axis, inverse_flattening ) );
}

/****************************************/
/*          Datum Factories             */
/****************************************/
Datum Datum::WGS84()
{
    return Datum( "WGS84", 6378137.0, 298.257223563 );
}

Datum Datum::GRS80()
{
    return Datum( "GRS80", 6378137.0, 298.257222101 );
}

Datum Datum::SPHERE()
{
    return Datum( "SPHERE", 6371000.0, 0.0 );
}

} // end of tmns::geo namespace
