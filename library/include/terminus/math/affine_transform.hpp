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
 * @file    affine_transform.hpp
 * @author  Marvin Smith
 * @date    11/15/2026
*/
#pragma once

// C++ Standard Libraries
#include <stdexcept>

// Terminus Libraries
#include <terminus/error.hpp>
#include <terminus/math/matrix.hpp>
#include <terminus/math/matrix/matrix_operations.hpp>
#include <terminus/math/point.hpp>
#include <terminus/outcome.hpp>

namespace tmns::math {

/**
 * @brief Matrix-backed affine pose transform for Dim-dimensional points.
 *
 * Thin wrapper over `Matrix<double,Dim+1,Dim+1>`: an affine transform is
 * position + orientation in one homogeneous matrix; applying it maps a
 * point "into" the pose frame and inverting it maps back out.  The
 * Dim=2 case is a 3x3 matrix (a GDAL geotransform's gt[6] array maps
 * directly onto it); Dim=3 is 4x4.
 */
template <int Dim>
class Affine_Transform
{
    public:

        /// Homogeneous matrix dimension.
        static constexpr int MATRIX_DIM = Dim + 1;

        /// Point type this transform maps.
        using PointT = Point_<double, Dim>;

        /// Backing matrix type.
        using MatrixT = Matrix<double, MATRIX_DIM, MATRIX_DIM>;

        /**
         * @brief Identity transform.
         */
        static Affine_Transform identity()
        {
            Affine_Transform result;
            result.m_matrix = MatrixT::identity();
            return result;
        }

        /**
         * @brief Build from a GDAL geotransform array.
         *
         * gt[6] = { origin_x, pixel_w, rot_x, origin_y, rot_y, pixel_h },
         * where geo_x = gt[0] + col*gt[1] + row*gt[2] and
         * geo_y = gt[3] + col*gt[4] + row*gt[5].
         */
        static Affine_Transform from_geotransform( const double gt[6] )
            requires( Dim == 2 )
        {
            auto result = identity();
            result.m_matrix[0][0] = gt[1];
            result.m_matrix[0][1] = gt[2];
            result.m_matrix[0][2] = gt[0];
            result.m_matrix[1][0] = gt[4];
            result.m_matrix[1][1] = gt[5];
            result.m_matrix[1][2] = gt[3];
            return result;
        }

        /**
         * @brief Build from a backing matrix.
         */
        static Affine_Transform from_matrix( const MatrixT& matrix )
        {
            Affine_Transform result;
            result.m_matrix = matrix;
            return result;
        }

        /**
         * @brief Map a point into the target frame.
         */
        PointT forward( const PointT& p ) const
        {
            PointT result;
            for( int i = 0; i < Dim; ++i )
            {
                double acc = m_matrix[i][Dim];
                for( int j = 0; j < Dim; ++j )
                {
                    acc += m_matrix[i][j] * p[j];
                }
                result[i] = acc;
            }
            return result;
        }

        /**
         * @brief Map a point back to the source frame.
         *
         * Exploits the affine structure [A|t; 0|1] — only the Dim x Dim
         * linear block is inverted, then result = A^-1 * (p - t).  Fails
         * with INVALID_INPUT when the linear block is singular.
         */
        Result<PointT> inverse( const PointT& p ) const
        {
            std::array<std::array<double, Dim>, Dim> a_inv;
            if( !invert_linear( a_inv ) )
            {
                return outcome::fail( error::Error_Code::INVALID_INPUT,
                                      "Affine_Transform linear block is singular" );
            }
            PointT result;
            for( int i = 0; i < Dim; ++i )
            {
                double acc = 0.0;
                for( int j = 0; j < Dim; ++j )
                {
                    acc += a_inv[i][j] * ( p[j] - m_matrix[j][Dim] );
                }
                result[i] = acc;
            }
            return outcome::ok<PointT>( result );
        }

        /**
         * @brief Matrix element access (row, column).
         */
        double operator()( int row, int col ) const
        {
            return m_matrix[row][col];
        }

        /**
         * @brief Backing matrix.
         */
        const MatrixT& matrix() const
        {
            return m_matrix;
        }

    private:

        /**
         * @brief Invert the Dim x Dim linear block in place via
         *        Gauss-Jordan elimination with partial pivoting.
         */
        bool invert_linear( std::array<std::array<double, Dim>, Dim>& out ) const
        {
            std::array<std::array<double, Dim>, Dim> a;
            for( int i = 0; i < Dim; ++i )
            {
                for( int j = 0; j < Dim; ++j )
                {
                    a[i][j] = m_matrix[i][j];
                }
                out[i] = {};
                out[i][i] = 1.0;
            }
            for( int col = 0; col < Dim; ++col )
            {
                int pivot = col;
                for( int row = col + 1; row < Dim; ++row )
                {
                    if( std::fabs( a[row][col] ) > std::fabs( a[pivot][col] ) )
                    {
                        pivot = row;
                    }
                }
                if( std::fabs( a[pivot][col] ) < 1e-12 )
                {
                    return false;
                }
                std::swap( a[col], a[pivot] );
                std::swap( out[col], out[pivot] );

                const double d = a[col][col];
                for( int j = 0; j < Dim; ++j )
                {
                    a[col][j]   /= d;
                    out[col][j] /= d;
                }
                for( int row = 0; row < Dim; ++row )
                {
                    if( row == col )
                    {
                        continue;
                    }
                    const double f = a[row][col];
                    for( int j = 0; j < Dim; ++j )
                    {
                        a[row][j]   -= f * a[col][j];
                        out[row][j] -= f * out[col][j];
                    }
                }
            }
            return true;
        }

        MatrixT m_matrix = MatrixT::identity();

}; // End of Affine_Transform Class

/// Common aliases.
using Affine_Transform_2D = Affine_Transform<2>;
using Affine_Transform_3D = Affine_Transform<3>;

} // end of tmns::math namespace
