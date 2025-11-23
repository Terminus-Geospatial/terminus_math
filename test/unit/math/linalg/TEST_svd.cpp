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
 * @file    TEST_svd.cpp
 * @author  Marvin Smith
 * @date    3/9/2024
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/math/linalg/svd.hpp>

using namespace tmns::math;

/************************************************/
/*          Test the Reduced SVD Method         */
/************************************************/
TEST( linalg_svd, svd_float )
{
    MatrixN<float> A( 4, 4, {  23,   1,  25,  98,
                              327,   2,  76,  66,
                              234,  26,  76, 662,
                               25,  62, 323,  23 } );

    // Find the least squares solution to the overconstrained problem Ax=b;
    auto s = linalg::svd( A );
    ASSERT_FALSE( s.has_error() );

    ASSERT_EQ( 4u, s.value().size() );
    ASSERT_NEAR( 744.6190185546875, s.value()[0], 1e-3 );
    ASSERT_NEAR( 336.4697156808418, s.value()[1], 1e-3 );
    ASSERT_NEAR( 262.4879718834515, s.value()[2], 1e-3 );
    ASSERT_NEAR( 5.838119793980186, s.value()[3], 1e-3 );

    A = MatrixN<float>( 3, 4, {  23,  1, 25,  98,
                                327,  2, 76,  66,
                                234, 26, 76, 662 } );

    // Find the least squares solution to the overconstrained problem Ax=b;
    s = linalg::svd( A );
    ASSERT_FALSE( s.has_error() );

    ASSERT_NEAR( 739.852951773669f, s.value()[0], 1e-3 );
    ASSERT_NEAR( 282.199549151858f, s.value()[1], 1e-3 );
    ASSERT_NEAR( 16.0319746218911f, s.value()[2], 1e-3 );

    A = MatrixN<float>( 4, 3, {  23,  1, 25,
                                327,  2, 76,
                                234, 26, 76,
                                25, 62, 323 });

    // Find the least squares solution to the overconstrained problem Ax=b;
    s = linalg::svd( A );
    ASSERT_FALSE( s.has_error() );

    ASSERT_NEAR( 444.78616, s.value()[0], 1e-3 );
    ASSERT_NEAR( 292.84455, s.value()[1], 1e-3 );
    ASSERT_NEAR( 16.649412, s.value()[2], 1e-3 );
}

/************************************************/
/*          Test the Reduced SVD Method         */
/************************************************/
TEST( linalg_svd, svd_double )
{
    MatrixN<double> A( 4, 4, {  23,   1,  25,  98,
                               327,   2,  76,  66,
                               234,  26,  76, 662,
                                25,  62, 323,  23 } );

    // Find the least squares solution to the overconstrained problem Ax=b;
    auto s = linalg::svd( A );
    ASSERT_FALSE( s.has_error() );

    ASSERT_NEAR( 744.6187691721427, s.value()[0], 1e-12 );
    ASSERT_NEAR( 336.4697156808418, s.value()[1], 1e-12 );
    ASSERT_NEAR( 262.4879718834515, s.value()[2], 1e-12 );
    ASSERT_NEAR( 5.838119793980186, s.value()[3], 1e-12 );

    A = MatrixN<double>( 3, 4, {  23,  1, 25,  98,
                                 327,  2, 76,  66,
                                 234, 26, 76, 662 } );

    // Find the least squares solution to the overconstrained problem Ax=b;
    s = linalg::svd( A );
    ASSERT_FALSE( s.has_error() );

    ASSERT_NEAR( 739.852951773669, s.value()[0], 1e-12 );
    ASSERT_NEAR( 282.199549151858, s.value()[1], 1e-12 );
    ASSERT_NEAR( 16.0319746218911, s.value()[2], 1e-12 );

    A = MatrixN<double>( 4, 3, {  23,  1, 25,
                                 327,  2, 76,
                                 234, 26, 76,
                                 25, 62, 323 } );

    // Find the least squares solution to the overconstrained problem Ax=b;
    s = linalg::svd( A );
    ASSERT_FALSE( s.has_error() );

    ASSERT_NEAR( 444.786309525272  , s.value()[0], 1e-12 );
    ASSERT_NEAR( 292.84455931980744, s.value()[1], 1e-12 );
    ASSERT_NEAR( 16.649412472420977, s.value()[2], 1e-12 );
}

/************************************************/
/*          Test the Reduced SVD Method         */
/************************************************/
TEST( linalg_svd, svd_complete_double_test_1 )
{
    MatrixN<double> A( 4, 4, {  23,   1,  25,  98,
                               327,   2,  76,  66,
                               234,  26,  76, 662,
                                25,  62, 323,  23 } );

    // Find the least squares solution to the overconstrained problem Ax=b;
    MatrixN<double> U, V;
    VectorN<double> S;
    auto svd_res = linalg::complete_svd( A, U, S, V );
    ASSERT_FALSE( svd_res.has_error() );

    ASSERT_NEAR( 744.6187691721427, S[0], 1e-12 );
    ASSERT_NEAR( 336.4697156808418, S[1], 1e-12 );
    ASSERT_NEAR( 262.4879718834515, S[2], 1e-12 );
    ASSERT_NEAR( 5.838119793980186, S[3], 1e-12 );

    // Convert diagonals to matrix
    auto S_mat = MatrixN<double>::identity( 4, 4 );
    S_mat(0,0) = S[0];
    S_mat(1,1) = S[1];
    S_mat(2,2) = S[2];
    S_mat(3,3) = S[3];

    // Check U and V
    MatrixN<double> U_exp( 4, 4, { 0.135722,  0.013262, -0.084548, -0.987044,
                                   0.285737, -0.441275,  0.849751, -0.039427,
                                   0.940352,  0.247012, -0.181056,  0.148129,
                                   0.125191, -0.862604, -0.487846,  0.047412 } );

    double U_diff = ( U_exp - U ).sum();
    ASSERT_NEAR( U_diff, 0, 0.0001 );

    MatrixN<double> V_exp( 4, 4, { 0.429387,  -0.320255,   0.843318,   0.043350,
                                   0.044208,  -0.142445,  -0.127011,   0.980624,
                                   0.184003,  -0.870965,  -0.414751,  -0.188530,
                                   0.883071,   0.344333,  -0.317278,  -0.030887 } );

    double V_diff = ( V_exp - V ).sum();
    ASSERT_NEAR( V_diff, 0, 0.001 );

    // Merge the results
    Matrix_4x4 Vt = transpose(V);
    auto orig = U * S_mat * Vt;
    auto orig_diff = (orig - A).sum();
    ASSERT_NEAR( orig_diff, 0, 0.001 );
}

/************************************************/
/*          Test the Reduced SVD Method         */
/************************************************/
TEST( linalg_svd, svd_complete_double_test_2 )
{
    Matrix<double> A( 2, 4, { -1, 1, 2,  4,
                               2, 0, 1, -7 } );

    // Find the least squares solution to the overconstrained problem Ax=b;
    MatrixN<double> U, V;
    VectorN<double> S;
    auto svd_res = linalg::complete_svd( A, U, S, V );
    ASSERT_FALSE( svd_res.has_error() );

    ASSERT_EQ( S.size(), 2 );
    ASSERT_NEAR( 8.381469500821092, S[0], 1e-12 );
    ASSERT_NEAR( 2.398117805030812, S[1], 1e-12 );

    // Check U and V
    MatrixN<double> U_exp( 2, 2, {  -0.501927,  -0.864910,
                                     0.864910,  -0.501927 } );

    double U_diff = ( U_exp - U ).sum();
    ASSERT_NEAR( U_diff, 0, 0.0001 );

    MatrixN<double> V_exp( 4, 4, { 0.266272,  -0.057939,  -0.689741,  0.670820,
                                  -0.059885,  -0.360662,  -0.645242, -0.670820,
                                  -0.016577,  -0.930624,   0.289246,  0.223607,
                                  -0.961893,   0.022454,  -0.155748,  0.223607 } );

    double V_diff = ( V_exp - V ).sum();
    ASSERT_NEAR( V_diff, 0, 0.001 );

    // Convert diagonals to matrix
    auto S_mat = MatrixN<double>::identity( U.cols(), V.rows() );
    S_mat(0,0) = S[0];
    S_mat(1,1) = S[1];

    // Merge the results
    Matrix_4x4 Vt = transpose(V);
    auto orig = U * S_mat * Vt;
    auto orig_diff = (orig - A).sum();
    ASSERT_NEAR( orig_diff, 0, 0.001 );

}