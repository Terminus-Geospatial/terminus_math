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
 * @file    quaternion.hpp
 * @author  Marvin Smith
 * @date    7/12/2023
*/
#pragma once

// Terminus Libraries
#include <terminus/math/matrix/matrix.hpp>
#include <terminus/math/vector/vector.hpp>

// C++ Libraries
#include <cmath>

namespace tmns::math {

class Quaternion
{
    public:

        using ElementT = double;

        using VectorT = Vector_<ElementT,3>;

        /**
         * Default Constructor
        */
        Quaternion() = default;

        /**
         * Parameterized Constructor
         * @param real
         * @param x
         * @param y
         * @param z
        */
        Quaternion( ElementT real,
                    ElementT x,
                    ElementT y,
                    ElementT z );

        /**
         * Parameterized Constructor
         */
        Quaternion( ElementT       real,
                    const VectorT& imag );


        /**
         * Get the real component
        */
        ElementT real() const;

        /**
         * Get the imaginary component
        */
        VectorT imag() const;

        /**
         * Indexing Operator
         */
        ElementT operator[]( size_t i ) const;

        /**
         * Function Operator
         */
        ElementT operator()( size_t i ) const;

        /**
         * Quaternion/Quaternion Multiplication
        */
        Quaternion operator * ( const Quaternion& rhs ) const;

        /**
         * Quaternion Division
         */
        Quaternion operator / ( const Quaternion& rhs ) const;

        /**
         * Get the magnitude of the quaternion
         * @note:  This really should be 1.
         */
        ElementT magnitude() const;

        /**
         * Get the squared magnitude
         * @note:  This really should be 1.
         */
        ElementT magnitude_sq() const;

        /**
         * Return a normalize quaternion.
         * This effectively makes it unit-length in case it isn't.
         */
        Quaternion normalize() const;

        /**
         * Get the conjugate of the quaternion
         */
        Quaternion conj() const;

        /**
         * @brief get the inverse of this quaternion
         */
        Quaternion inverse() const;

        /**
         * Build a rotation matrix from a quaternion
         */
        template <typename MatrixT>
        void rotation_matrix( Matrix_Base<MatrixT>& rmat ) const
        {
            MatrixT rot = rmat.impl();
            ElementT w2 = real()     * real();
            ElementT x2 = imag().x() * imag().x();
            ElementT y2 = imag().y() * imag().y();
            ElementT z2 = imag().z() * imag().z();
            ElementT wx = real()     * imag().x();
            ElementT wy = real()     * imag().y();
            ElementT wz = real()     * imag().z();
            ElementT xy = imag().x() * imag().y();
            ElementT yz = imag().y() * imag().z();
            ElementT zx = imag().z() * imag().x();

            rot(0,0) = w2 + x2 - y2 - z2;
            rot(1,1) = w2 - x2 + y2 - z2;
            rot(2,2) = w2 - x2 - y2 + z2;
            rot(0,1) = 2 * (xy - wz);
            rot(0,2) = 2 * (zx + wy);
            rot(1,2) = 2 * (yz - wx);
            rot(1,0) = 2 * (xy + wz);
            rot(2,0) = 2 * (zx - wy);
            rot(2,1) = 2 * (yz + wx);
        }

        Matrix<ElementT,3,3> to_rotation_matrix() const
        {
            auto rot = Matrix<ElementT,3,3>::identity();
            rotation_matrix( rot );
            return rot;
        }

        Matrix<ElementT,4,4> rotation_matrix_4() const
        {
            auto rot = Matrix<ElementT,4,4>::identity();
            rotation_matrix( rot );
            return rot;
        }

        /**
         * Build a quaternion from a rotation matrix
         */
        template <typename MatrixT>
        static Quaternion from_matrix( const MatrixT& mat )
        {
            // Get diagonal parts
            auto diag = mat.diagonal();

            ElementT ww = 1.0 + diag[0] + diag[1] + diag[2];
            ElementT xx = 1.0 + diag[0] - diag[1] - diag[2];
            ElementT yy = 1.0 - diag[0] + diag[1] - diag[2];
            ElementT zz = 1.0 - diag[0] - diag[1] + diag[2];

            ElementT max_val = std::max( ww, std::max( xx, std::max( yy, zz ) ) );

            // Depending on the max, we need to determine how to parse the quaternion
            std::array<ElementT,4> components;

            static constexpr ElementT EPS { 0.00001 };

            if( std::fabs( ww - max_val ) < EPS )
            {
                ElementT w4 = std::sqrt( ww * 4.0 );
                components[0] = w4 / 4.0;
                components[1] = (mat(2,1) - mat(1,2)) / w4;
                components[2] = (mat(0,2) - mat(2,0)) / w4;
                components[3] = (mat(1,0) - mat(0,1)) / w4;
            }

            else if( std::fabs( xx - max_val ) < EPS )
            {
                ElementT x4 = std::sqrt( xx * 4.0 );
                components[0] = ( mat(2,1) - mat(1,2) ) / x4;
                components[1] = x4 / 4;
                components[2] = ( mat(0,1) + mat(1,0) ) / x4;
                components[3] = ( mat(0,2) + mat(2,0) ) / x4;
            }

            else if( std::fabs( yy - max_val) < EPS )
            {
                ElementT y4 = std::sqrt(yy * 4.0);
                components[0] = ( mat(0,2) - mat(2,0) ) / y4;
                components[1] = ( mat(0,1) + mat(1,0) ) / y4;
                components[2] =  y4 / 4;
                components[3] = ( mat(1,2) + mat(2,1) ) / y4;
            }

            else
            {
                ElementT z4 = std::sqrt( zz * 4.0 );
                components[0] = ( mat(1,0) - mat(0,1) ) / z4;
                components[1] = ( mat(0,2) + mat(2,0) ) / z4;
                components[2] = ( mat(1,2) + mat(2,1) ) / z4;
                components[3] =  z4 / 4;
            }

            return Quaternion( components[0],
                               components[1],
                               components[2],
                               components[3] );
        }

        /**
         * Rotate a Vector by the quaternion.
         *
         * Rotation is notated by (this) Q1, [r, i1, i2, i3]
         *                         (v)   Q1, [0, v1, v2, v3]
         *
         * Perform (Q1 x Q2)/Q1, return imaginary component.
         */
        template<typename InVectorT>
        Vector_<ElementT,3> rotate_vector( const Vector_Base<InVectorT>& in_vec ) const
        {
            // Get underlying vector type
            const InVectorT& v = in_vec.impl();
            return ( *this * Quaternion( 0, v[0], v[1], v[2] ) / *this ).imag();
        }

    private:

        /// Real Component
        ElementT m_real { 0 };

        /// Imaginary Component
        VectorT m_imag { { 1, 0, 0 } };

}; // End of Quaternion Class

} // end of tmns::math API