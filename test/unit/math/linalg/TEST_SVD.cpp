/**
 * @file    TEST_SVD.cpp
 * @author  Marvin Smith
 * @date    3/9/2024
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/math/linalg/SVD.hpp>

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

    // Check U and V
    MatrixN<double> U_exp( 4, 4, { 0.1357, -0.0133, 0.0845, -0.9870,
                                   0.2857,  0.4413,-0.8498, -0.0394,
                                   0.9404, -0.2470, 0.1811,  0.1481,
                                   0.1252,  0.8626, 0.4878,  0.0474 } );

    double U_diff = ( U_exp - U ).sum();
    ASSERT_NEAR( U_diff, 0, 0.0001 );

    MatrixN<double> V_exp( 4, 4, { 0.4294, 0.3203, -0.8433, 0.0433,
                                   0.0442, 0.1424,  0.1270, 0.9806,
                                   0.1840, 0.8710,  0.4148,-0.1885,
                                   0.8831,-0.3443,  0.3173,-0.0309 } );
    
    double V_diff = ( V_exp - V ).sum();
    ASSERT_NEAR( V_diff, 0, 0.001 );
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
    MatrixN<double> U_exp( 2, 2, {  0.501927, -0.864910,
                                   -0.864910, -0.501927 } );

    double U_diff = ( U_exp - U ).sum();
    ASSERT_NEAR( U_diff, 0, 0.0001 );

    MatrixN<double> V_exp( 4, 4, { -0.266272, -0.057939, -0.145198,  0.951136,
                                    0.059885, -0.360662, -0.919322, -0.145547,
                                    0.016577, -0.930624,  0.365581,  0.003761,
                                    0.961893,  0.022454,  0.010741,  0.272290 } );
    
    double V_diff = ( V_exp - V ).sum();
    ASSERT_NEAR( V_diff, 0, 0.001 );

    // Convert diagonals to matrix
    auto S_mat = MatrixN<double>::identity( 2, 2 );
    S_mat(0,0) = S[0];
    S_mat(1,1) = S[1];

    // Merge the results
    auto merged = U * S_mat * transpose( V );

    std::cout << "merged:\n" << merged.to_log_string() << std::endl;
    
}