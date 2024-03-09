/**
 * @file    SVD.cpp
 * @author  Marvin Smith
 * @date    11/13/2023
*/
#include <terminus/math/linalg/SVD.hpp>

// Project Libraries
#include "../../thirdparty/eigen/Eigen_Utilities.hpp"

// Eigen Libraries
#include <Eigen/SVD>

namespace tmns::math::linalg {

/**************************************/
/*        Get singular values         */
/**************************************/
ImageResult<VectorN<double>> svd_impl( const MatrixN<double>& mat_A )
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

    VectorN<double> output_tmns( s.data(),
                                 s.size() );

    return outcome::ok<VectorN<double>>( output_tmns );
}

/**************************************/
/*        Get singular values         */
/**************************************/
ImageResult<VectorN<float>> svd_impl( const MatrixN<float>& mat_A )
{
    // Convert to eigen type
    auto A = eigen::to_eigen<::Eigen::MatrixXf>( mat_A );

    // Create the Jacobian SVN Engine
    Eigen::JacobiSVD<Eigen::Matrix<float,
                                   Eigen::Dynamic,
                                   Eigen::Dynamic,
                                   Eigen::RowMajor> > svd( A.value(),
                                                           Eigen::ComputeThinU |
                                                           Eigen::ComputeThinV );
    
    // Grab the Diagonal
    Eigen::Matrix<float,
                  Eigen::Dynamic,
                  Eigen::Dynamic,
                  Eigen::RowMajor> s = svd.singularValues().asDiagonal();

    VectorN<float> output_tmns( s.data(),
                                s.size() );

    return outcome::ok<VectorN<float>>( output_tmns );
}


/**************************************/
/*        Get singular values         */
/**************************************/
ImageResult<void> complete_svd_impl( const MatrixN<double>& mat_A,
                                     MatrixN<double>&       U,
                                     VectorN<double>&       S,
                                     MatrixN<double>&       V )
{
    // Convert to eigen type
    auto A = eigen::to_eigen<::Eigen::MatrixXd>( mat_A );

    // Create the Jacobian SVN Engine
    Eigen::JacobiSVD<Eigen::Matrix<double,
                                   Eigen::Dynamic,
                                   Eigen::Dynamic,
                                   Eigen::RowMajor> > svd( A.value(),
                                                           Eigen::ComputeFullU |
                                                           Eigen::ComputeFullV );
    
    // Grab U and V
    ::Eigen::MatrixXd u = svd.matrixU();
    U = eigen::from_eigen<MatrixN<double>>( u ).value();

    ::Eigen::MatrixXd v = svd.matrixV();
    V = eigen::from_eigen<MatrixN<double>>( v ).value();
    
    // Grab the Diagonal
    Eigen::Matrix<double,
                  Eigen::Dynamic,
                  Eigen::Dynamic,
                  Eigen::RowMajor> s = svd.singularValues().asDiagonal();

    S = eigen::from_eigen<VectorN<double>>( s ).value();

    return outcome::ok();
}

/**************************************/
/*        Get singular values         */
/**************************************/
ImageResult<void> complete_svd_impl( const MatrixN<float>& mat_A,
                                     MatrixN<float>&       U,
                                     VectorN<float>&       S,
                                     MatrixN<float>&       V )
{
    // Convert to eigen type
    auto A = eigen::to_eigen<::Eigen::MatrixXf>( mat_A );

    // Create the Jacobian SVN Engine
    Eigen::JacobiSVD<Eigen::Matrix<float,
                                   Eigen::Dynamic,
                                   Eigen::Dynamic,
                                   Eigen::RowMajor> > svd( A.value(),
                                                           Eigen::ComputeFullU |
                                                           Eigen::ComputeFullV );
    
    // Grab U and V
    ::Eigen::MatrixXf u = svd.matrixU();
    U = eigen::from_eigen<MatrixN<float>>( u ).value();

    ::Eigen::MatrixXf v = svd.matrixV();
    V = eigen::from_eigen<MatrixN<float>>( v ).value();
    
    // Grab the Diagonal
    Eigen::Matrix<float,
                  Eigen::Dynamic,
                  Eigen::Dynamic,
                  Eigen::RowMajor> s = svd.singularValues().asDiagonal();

    S = eigen::from_eigen<VectorN<float>>( s ).value();

    return outcome::ok();
}

} // End of tmns::math::linalg namespace