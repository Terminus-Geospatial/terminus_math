/**************************** INTELLECTUAL PROPERTY RIGHTS ****************************/
/*                                                                                    */
/*                           Copyright (c) 2024 Terminus LLC                          */
/*                                                                                    */
/*                                All Rights Reserved.                                */
/*                                                                                    */
/*          Use of this source code is governed by LICENSE in the repo root.          */
/*                                                                                    */
/***************************# INTELLECTUAL PROPERTY RIGHTS ****************************/
/**
 * @file    quaternion_utilities.cpp
 * @author  Marvin Smith
 * @date    9/16/2023
 */
#include <terminus/math/quaternion_utilities.hpp>

// C++ Libraries
#include <cmath>
#include <numeric>

namespace tmns::math {

/****************************************************/
/*          Spherical Linear Interpolation          */
/****************************************************/
Quaternion slerp( double             alpha,
                  const Quaternion&  a,
                  const Quaternion&  b,
                  int                spin )
{
    const double SLERP_EPSILON = 1.0E-6;              // a tiny number
    double beta;                      // complementary interp parameter
    double theta;                     // angle between A and B
    double sin_t, cos_t;              // sine, cosine of theta
    double phi;                       // theta plus spins
    int bflip;                        // use negation of B?

    // cosine theta = dot product of A and B
    cos_t = a.real() * b.real() + Vector3d::dot( a.imag(), b.imag() );

    // if B is on opposite hemisphere from A, use -B instead
    if( cos_t < 0.0 )
    {
        cos_t = -cos_t;
        bflip = true;
    }
    else
    {
        bflip = false;
    }

    // if B is (within precision limits) the same as A,
    // just linear interpolate between A and B.
    // Can't do spins, since we don't know what direction to spin.
    if( 1.0 - cos_t < SLERP_EPSILON )
    {
        beta = 1.0 - alpha;
    }

    // normal case
    else
    {
        theta = std::acos( cos_t );
        phi   = theta + spin * M_PI;
        sin_t = std::sin( theta );
        beta  = std::sin( theta - alpha * phi ) / sin_t;
        alpha = std::sin( alpha * phi ) / sin_t;
    }

    if (bflip)
    {
        alpha = -alpha;
    }

    // interpolate
    return Quaternion( beta * a.real()     + alpha * b.real(),
	                   beta * a.imag().x() + alpha * b.imag().x(),
	                   beta * a.imag().y() + alpha * b.imag().y(),
	                   beta * a.imag().z() + alpha * b.imag().z() );
}

/************************************************/
/*          Compute the n-weight slerp          */
/************************************************/
Quaternion slerp_n( std::vector<double>     w,
                    std::vector<Quaternion> Q,
                    int                     spin )
{
    if( w.size() != Q.size() )
    {
        std::cerr << "Expecting as many quaternions as weights.";
        return Quaternion();
    }
    if( w.empty() )
    {
        std::cerr << "Must have at least one quaternion in the list.";
        return Quaternion();
    }

    if( Q.size() == 1 )
    {
        return Q[0];
    }

    auto sum = std::accumulate( w.begin(),
                                w.end(),
                                0 );
    if( std::fabs( sum - 1 ) > 0.001 )
    {
        std::cout << "Sum of weights is not equal to 1. Actual: " << sum;
        return Quaternion();
    }

    // Standard Slep
    if( Q.size() == 2 )
    {
        if( w[0] < 0 || w[1] < 0 )
        {
            std::cerr << "Expecting the weights to be >= 0 and sum up to 1.";
            return Quaternion();
        }
        return slerp( w[1], Q[0], Q[1], spin );
    }

    // Call recursively this function with fewer terms
    double sum2 = w[0] + w[1];
    if( sum2 == 0 ) sum2 = 1.0;

    Quaternion q = slerp( w[1] / sum, Q[0], Q[1], spin );
    std::vector<double>     w2 = w;
    std::vector<Quaternion> Q2 = Q;
    w2.erase( w2.begin() );
    Q2.erase( Q2.begin() );
    w2[0] = sum;
    Q2[0] = q;
    return slerp_n( w2, Q2, spin );
}

} // End of tmns::math namespace