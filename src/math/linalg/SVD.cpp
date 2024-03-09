/**
 * @file    SVD.cpp
 * @author  Marvin Smith
 * @date    11/13/2023
*/
#include <terminus/math/linalg/SVD.hpp>

namespace tmns::math::linalg {

/**************************************/
/*        Get singular values         */
/**************************************/
ImageResult<VectorN<double>> svd_impl( const MatrixN<double>& A )
{
    // Convert to eigen type
    auto A = eigen::to_eigen<::Eigen::MatrixXd>( mat_A );

    // Create the Jacobian SVN Engine
    Eigen::JacobiSVD<Eigen::Matrix<double,
                                   Eigen::Dynamic,
                                   Eigen::Dynamic,
                                   Eigen::RowMajor> > svd( A.value(),
                                                           Eigen::ComputeThinU |
                                                           Eigen::ComputeThinV );
    
    // Grab the Diagonal
    Eigen::Matrix<double,
                  Eigen::Dynamic,
                  Eigen::Dynamic,
                  Eigen::RowMajor> s = svd.singularValues().asDiagonal();

    Eigen::MatrixXd output = inv * b.value();

    VectorN<double> output_tmns( output.data(),
                                 output.size() );

    return outcome::ok<VectorN<double>>( output_tmns );
}

/**************************************/
/*        Get singular values         */
/**************************************/
ImageResult<VectorN<double>> svd_impl( const MatrixN<double>& A )
{
    // Convert to eigen type
    auto A = eigen::to_eigen<::Eigen::MatrixXd>( mat_A );

    // Create the Jacobian SVN Engine
    Eigen::JacobiSVD<Eigen::Matrix<double,
                                   Eigen::Dynamic,
                                   Eigen::Dynamic,
                                   Eigen::RowMajor> > svd( A.value(),
                                                           Eigen::ComputeThinU |
                                                           Eigen::ComputeThinV );
    
    // Grab the Diagonal
    Eigen::Matrix<double,
                  Eigen::Dynamic,
                  Eigen::Dynamic,
                  Eigen::RowMajor> s = svd.singularValues().asDiagonal();

    Eigen::MatrixXd output = inv * b.value();

    VectorN<double> output_tmns( output.data(),
                                 output.size() );

    return outcome::ok<VectorN<double>>( output_tmns );
}


} // End of tmns::math::linalg namespace