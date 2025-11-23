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
 * @file    SVD.cpp
 * @author  Marvin Smith
 * @date    11/13/2023
*/
#include <terminus/math/linalg/SVD.hpp>

// Project Libraries
#include "../thirdparty/eigen/eigen_utilities.hpp"
#include "../thirdparty/opencv/opencv_utilities.hpp"

// OpenCV Libraries
#include <opencv4/opencv2/core.hpp>

namespace tmns::math::linalg {

/**************************************/
/*        Get singular values         */
/**************************************/
Result<VectorN<double>> svd( const MatrixN<double>& mat_A )
{
    // Convert to eigen type
    auto A = ocv::to_opencv<double>( mat_A );

    // Compute Matrices
    cv::Mat w, u, vt;
    cv::SVD::compute( A, w, u, vt );

    VectorN<double> output_tmns( w.ptr<double>(), w.total() );

    return outcome::ok<VectorN<double>>( output_tmns );
}

/**************************************/
/*        Get singular values         */
/**************************************/
Result<VectorN<float>> svd( const MatrixN<float>& mat_A )
{
    // Convert to eigen type
    auto A = ocv::to_opencv<float>( mat_A );

    // Compute Matrices
    cv::Mat w, u, vt;
    cv::SVD::compute( A, w, u, vt );

    return outcome::ok<VectorN<float>>( w.ptr<float>(), w.total() );
}


/**************************************/
/*        Get singular values         */
/**************************************/
Result<void> complete_svd_impl( const MatrixN<double>& mat_A,
                                MatrixN<double>&       U,
                                VectorN<double>&       S,
                                MatrixN<double>&       V )
{
    // Convert to eigen type
    auto A = ocv::to_opencv<double>( mat_A );

    // Compute Matrices
    cv::Mat w, u, vt;
    cv::SVD::compute( A, w, u, vt, cv::SVD::FULL_UV );

    // Grab U and V
    U = ocv::from_opencv<MatrixN<double>>( u );

    V = ocv::from_opencv<MatrixN<double>>( vt.t() );

    S = ocv::from_opencv<VectorN<double>>( w );

    return outcome::ok();
}

/**************************************/
/*        Get singular values         */
/**************************************/
Result<void> complete_svd_impl( const MatrixN<float>& mat_A,
                                MatrixN<float>&       U,
                                VectorN<float>&       S,
                                MatrixN<float>&       V )
{
    // Convert to eigen type
    auto A = ocv::to_opencv<float>( mat_A );

    // Compute Matrices
    cv::Mat w, u, vt;
    cv::SVD::compute( A, w, u, vt, cv::SVD::FULL_UV );

    // Grab U and V
    U = ocv::from_opencv<MatrixN<float>>( u );

    V = ocv::from_opencv<MatrixN<float>>( vt.t() );

    S = ocv::from_opencv<VectorN<float>>( w );

    return outcome::ok();
}

} // End of tmns::math::linalg namespace