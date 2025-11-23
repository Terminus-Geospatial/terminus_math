/**
 * @file    Quaternion.cpp
 * @author  Marvin Smith
 * @date    7/12/2023
*/
#include <terminus/math/Quaternion.hpp>

// Terminus Libraries
#include <terminus/math/vector.hpp>

// C++ Libraries
#include <cmath>

namespace tmns::math {

/********************************************/
/*          Parameterized Constructor       */
/********************************************/
Quaternion::Quaternion( ElementT real,
                        ElementT x,
                        ElementT y,
                        ElementT z )
 : m_real( real ),
   m_imag( ToVector3<ElementT>( x, y, z ) )
{
}

/********************************************/
/*          Parameterized Constructor       */
/********************************************/
Quaternion::Quaternion( ElementT       real,
                        const VectorT& imag )
 : m_real( real ),
   m_imag( imag )
{
}

/**********************************/
/*      Get the Real Component    */
/**********************************/
Quaternion::ElementT Quaternion::real() const
{
    return m_real;
}

/****************************************/
/*      Get the Imaginary Component     */
/****************************************/
Quaternion::VectorT Quaternion::imag() const
{
    return m_imag;
}

/********************************/
/*      Indexing Operator       */
/********************************/
Quaternion::ElementT Quaternion::operator[]( size_t i ) const
{
    switch ( i )
    {
        case 0: return real(); break;
        case 1:
        case 2:
        case 3:
            return imag()[ i - 1 ];
        default:
        {
            std::stringstream sout;
            sout << "Quaternion: Invalid index: " << i;
            throw std::runtime_error( sout.str() );
        }
    }
}

/********************************/
/*      Function Operator       */
/********************************/
Quaternion::ElementT Quaternion::operator() ( size_t i ) const
{
    return this->operator[]( i );
}

/************************************/
/*      Multiplication Operator     */
/************************************/
Quaternion Quaternion::operator * ( const Quaternion& rhs ) const
{
    // set the real and imaginary components
    Quaternion output;
    output.m_real = real() * rhs.real() - VectorT::dot( imag(), rhs.imag() );
    output.m_imag = real()     * rhs.imag() 
                  + rhs.real() * rhs.imag() 
                  + VectorT::cross( imag(), rhs.imag() );
    return output;
}

/****************************************/
/*          Quaternion Division         */
/****************************************/
Quaternion Quaternion::operator / ( const Quaternion& rhs ) const
{
    // Reference:  https://www.boost.org/doc/libs/1_83_0/boost/math/quaternion.hpp
    //      Operator /=
    auto denom = magnitude();

    auto real = ( m_real * rhs.m_real + Vector3d::dot( m_imag, rhs.m_imag ) ) / denom;
    Vector3d imag;
    imag.x() = ( -m_real * rhs.m_imag.x() ) + ( m_imag.x() * rhs.m_real     ) + ( m_imag.y() * rhs.m_imag.z() ) + ( m_imag.z() * rhs.m_imag.y() );
    imag.y() = ( -m_real * rhs.m_imag.y() ) + ( m_imag.x() * rhs.m_imag.z() ) + ( m_imag.y() * rhs.m_real     ) - ( m_imag.z() * rhs.m_imag.x() );
    imag.z() = ( -m_real * rhs.m_imag.z() ) - ( m_imag.x() * rhs.m_imag.y() ) + ( m_imag.y() * rhs.m_imag.x() ) + ( m_imag.z() * rhs.m_real     );
    
    return Quaternion( real, imag / denom );
}

/************************************/
/*      Quaternion magnitude        */
/************************************/
Quaternion::ElementT Quaternion::magnitude() const
{
    return std::sqrt( magnitude_sq() );
}

/********************************/
/*      Squared Magnitude       */
/********************************/
Quaternion::ElementT Quaternion::magnitude_sq() const
{
    return ( m_real * m_real ) + m_imag.magnitude_sq();
}

/****************************************/
/*      Normalize the Quaternion        */
/****************************************/
Quaternion Quaternion::normalize() const
{
    auto mag = magnitude();
    return Quaternion( m_real / mag, m_imag / mag );
}

/****************************************/
/*          Quaternion Conjugate        */
/****************************************/
Quaternion Quaternion::conj() const
{
    return Quaternion( m_real, m_imag * -1 );
}

/****************************************/
/*          Quaternion Inverse          */
/****************************************/
Quaternion Quaternion::inverse() const
{
    return conj() / normalize();
}

} // end of tmns::math API