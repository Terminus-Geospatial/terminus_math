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
 * @file    TEST_levenburg_marquardt.cpp
 * @author  Marvin Smith
 * @date    10/15/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/math/matrix/matrixn.hpp>
#include <terminus/math/optimization/levenburg_marquardt.hpp>
#include <terminus/math/vector/sub_vector.hpp>

namespace tmx = tmns::math;

/**
 * Create test LM model
*/
struct Test_Least_Squares_Model : public tmx::optimize::Least_Squares_Model_Base<Test_Least_Squares_Model>
{
    using result_type   = tmx::VectorN<double>;
    using domain_type   = tmx::VectorN<double>;
    using jacobian_type = tmx::MatrixN<double>;

    Test_Least_Squares_Model() = default;

    /// Evaluate h(x)
    result_type operator()( domain_type const& x ) const
    {
        // For now make up a function to get started with
        tmx::VectorN<double> h(5);
        h(0) = std::sin(x(0)+0.1);
        h(1) = std::cos(x(1) * x(2));
        h(2) = x(1) * std::cos(x(2));
        h(3) = std::atan2(x(0),x(3));
        h(4) = std::atan2(x(2),x(1));
        return h;
    }
}; // End of Test_Least_Squares_Model class

TEST( Levenberg_Marquardt, least_squares_model )
{

    Test_Least_Squares_Model model;

    tmx::Vector_<double,5> z;
    tmx::Vector_<double,4> x( { 0.2, 0.3, 0.4, 0.5 } );
    tmx::Vector_<double,5> expected_first( { 0.29552020666133957510,
                                             0.99280863585386625224,
                                             0.27631829820086552483,
                                             0.38050637711236488630,
                                             0.92729521800161223242 } );

    auto first = model( x );

    ASSERT_EQ( expected_first.size(), first.size() );
    for( size_t i = 0; i < expected_first.size(); i++ )
    {
        ASSERT_NEAR( expected_first[i],
                     first[i],
                     0.01 );
    }

    auto jacobian = model.jacobian(x);

    //TODO: Vet this.
    EXPECT_NEAR( -0.0478849, jacobian(1,1), 1e-5 );
    EXPECT_NEAR( -0.116826,  jacobian(2,2), 1e-5 );
    EXPECT_NEAR(  1.72414,   jacobian(3,0), 1e-5 );
}

/************************************/
/*          Test the LM Solver      */
/************************************/
TEST( Levenberg_Marquardt, levenberg_marquardt )
{
    Test_Least_Squares_Model model;
    tmx::VectorN<double> target(5);
    target(0) = 0.2;
    target(1) = 0.3;
    target(2) = 0.4;
    target(3) = 0.5;
    target(4) = 0.6;

    tmx::VectorN<double> seed(4);
    seed[0] = 1.0;
    seed[1] = 1.0;
    seed[2] = 1.0;
    seed[3] = 1.0;

    tmx::optimize::LM_STATUS_CODE status;
    auto best = tmx::optimize::levenberg_marquardt( model, seed, target, status );
    if( best.has_error() )
    {
        tmns::log::error( ADD_CURRENT_LOC(), "Test is about to fail: ", best.error().message() );
    }
    ASSERT_FALSE( best.has_error());

    tmx::Vector_<double,4> expected_best( { 0.101358, 1.15485, 1.12093, 0.185534 } );

    ASSERT_EQ( tmx::optimize::LM_STATUS_CODE::ERROR_CONVERGED_REL_TOLERANCE, status );

    EXPECT_NEAR( tmx::VectorN<double>( expected_best - best.value() ).magnitude(), 0, 1e-5 );
}