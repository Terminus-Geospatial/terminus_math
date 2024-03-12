/**
 * @file    TEST_Eigen_Utilities.cpp
 * @author  Marvin Smith
 * @date    3/9/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/math/matrix.hpp>
#include <terminus/math/thirdparty/eigen/Eigen_Utilities.hpp>

using namespace tmns::math;

/**
 *  Test the To/From Eigen (Matrix)
*/
TEST( thirdparty_eigen_utilities, to_from_eigen_matrix_float_01 )
{
    // Create a Terminus Matrix
    MatrixN<float> A( 4, 4, {  23,   1,  25,  98,
                              327,   2,  76,  66,
                              234,  26,  76, 662,
                               25,  62, 323,  23 } );
    
    // Convert the Terminus matrix into an Eigen type
    auto resA = eigen::to_eigen<::Eigen::MatrixXf>( A );
    ::Eigen::MatrixXf eigenA = resA.value();
    std::cout << "Eigen Matrix:\n" << eigenA << std::endl;

    // Convert back to 
    auto outResA = eigen::from_eigen<MatrixN<float>>( eigenA );
    MatrixN<float> B = outResA.value();
    std::cout << "Result Matrix:\n" << B.to_log_string() << std::endl;

    auto delta = A - B;
    tmns::log::info( delta.to_log_string() );
}

TEST( thirdparty_eigen_utilities, to_from_eigen_matrix_float_02 )
{
    // Create a Terminus Matrix
    MatrixN<float> A( 3, 4, {  23,  1, 25,  98,
                                327,  2, 76,  66,
                                234, 26, 76, 662 } );
    
    // Convert the Terminus matrix into an Eigen type
    auto resA = eigen::to_eigen<::Eigen::MatrixXf>( A );
    ::Eigen::MatrixXf eigenA = resA.value();
    std::cout << "Eigen Matrix:\n" << eigenA << std::endl;

    // Convert back to 
    auto outResA = eigen::from_eigen<MatrixN<float>>( eigenA );
    MatrixN<float> B = outResA.value();
    std::cout << "Result Matrix:\n" << B.to_log_string() << std::endl;

    auto delta = A - B;
    tmns::log::info( delta.to_log_string() );
}
