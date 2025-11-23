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
 * @file    Solvers.cpp
 * @author  Marvin Smith
 * @date    10/15/2023
 */
#include <terminus/math/linalg/Solvers.hpp>

// Project Libraries
#include "../thirdparty/eigen/Eigen_Utilities.hpp"

// Eigen Libraries
#include <Eigen/SVD>

namespace tmns::math::linalg {

/************************************************************/
/*          Solve the system of linear equations            */
/************************************************************/
Result<VectorN<double>> solve_symmetric( const MatrixN<double>& A,
                                         const VectorN<double>& b )
{
    tmns::log::info( ADD_CURRENT_LOC(), "A: ", A.to_log_string() );
    tmns::log::info( ADD_CURRENT_LOC(), "b: ", b.to_log_string() );

    using EigenMatrixT = eigen::TMNS_to_Eigen_Picker<MatrixN<double>>::EigenT;
    using EigenVectorT = eigen::TMNS_to_Eigen_Picker<VectorN<double>>::EigenT;

    // Create Eigen matrix
    auto eigen_A = eigen::to_eigen<EigenMatrixT>( A );
    auto eigen_b = eigen::to_eigen<EigenVectorT>( b );

    Eigen::MatrixXd x = eigen_A.value().colPivHouseholderQr().solve( eigen_b.value() );

    std::cout << x << std::endl;
    std::cout << "size: " << x.size() << std::endl;
    return outcome::ok<VectorN<double>>( VectorN<double>( x.data(),
                                                          static_cast<size_t>(x.size()) ) );
}

/************************************************/
/*      Solve System of Linear Equations        */
/************************************************/
Result<VectorN<double>> solve( const MatrixN<double>& mat_A,
                               const VectorN<double>& vec_b,
                               double                 eps )
{
    using EigenMatrixT = eigen::TMNS_to_Eigen_Picker<MatrixN<double>>::EigenT;
    using EigenVectorT = eigen::TMNS_to_Eigen_Picker<VectorN<double>>::EigenT;

    // Convert to eigen types
    auto A = eigen::to_eigen<EigenMatrixT>( mat_A );
    auto b = eigen::to_eigen<EigenVectorT>( vec_b );

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

    // wipe out the small singular values
    for( int64_t y = 0; y < s.rows(); y++ )
    for( int64_t x = 0; x < s.cols(); x++ )
    {
        if( s(y,x) > eps )
        {
            s(y,x) = 1/s(y,x);
        }
        else
        {
            s(y,x) = 0;
        }
    }

    auto inv = svd.matrixV() * s * svd.matrixU().transpose();

    Eigen::MatrixXd output = inv * b.value();

    VectorN<double> output_tmns( output.data(),
                                 static_cast<size_t>(output.size()) );

    return outcome::ok<VectorN<double>>( output_tmns );
}

} // End of tmns::math::linalg namespace