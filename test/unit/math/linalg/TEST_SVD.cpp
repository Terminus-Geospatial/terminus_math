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