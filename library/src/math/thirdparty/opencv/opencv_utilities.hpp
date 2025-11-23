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
 * @file    OpenCV_Utilities.hpp
 * @author  Marvin Smith
 * @date    3/18/2024
*/
#pragma once

// Terminus Libraries
#include <terminus/math/matrix.hpp>

// OpenCV Libraries
#include <opencv4/opencv2/core.hpp>

namespace tmns::math::ocv {

/**
 * Base Picker Type
*/
template <typename ValueT>
struct OpenCV_Picker
{
    static constexpr int type_code { CV_8U };

}; // End of OpenCV_Picker struct

template <>
struct OpenCV_Picker<double>
{
    static constexpr int type_code { CV_64F };

}; // End of OpenCV_Picker struct

template <>
struct OpenCV_Picker<float>
{
    static constexpr int type_code { CV_32F };

}; // End of OpenCV_Picker struct

/**
 * Convert Terminus Matrix to OpenCV Matrix
*/
template <typename ValueT>
cv::Mat to_opencv( const MatrixN<ValueT>& mat )
{
    cv::Mat output( static_cast<int>(mat.rows()),
                    static_cast<int>(mat.cols()),
                    OpenCV_Picker<ValueT>::type_code );

    for( size_t r = 0; r < mat.rows(); r++ )
    for( size_t c = 0; c < mat.cols(); c++ )
        output.at<ValueT>(static_cast<int>(r),static_cast<int>(c)) = mat(r,c);

    return output;
}

/**
 * Convert OpenCV Matrix to Terminus Matrix
*/
template <typename ReturnT>
ReturnT from_opencv( const cv::Mat& mat )
    requires (std::is_same<ReturnT,MatrixN<typename ReturnT::value_type>>::value)
{
    ReturnT output( static_cast<size_t>(mat.rows),
                    static_cast<size_t>(mat.cols) );

    for( int r = 0; r < mat.rows; r++ )
    for( int c = 0; c < mat.cols; c++ )
        output(static_cast<size_t>(r),static_cast<size_t>(c)) = mat.at<typename ReturnT::value_type>(r,c);

    return output;
}

/**
 * Convert OpenCV Matrix to Terminus Matrix
*/
template <typename VectorT>
VectorT from_opencv( const cv::Mat& mat )
    requires (std::is_same<VectorT,VectorN<typename VectorT::value_type>>::value)
{
    VectorT output( mat.total() );

    size_t i = 0;
    for( int r = 0; r < mat.rows; r++ )
    for( int c = 0; c < mat.cols; c++ )
        output[i++] = mat.at<typename VectorT::value_type>(r,c);

    return output;
}



} // End of tmns::math::ocv namespace