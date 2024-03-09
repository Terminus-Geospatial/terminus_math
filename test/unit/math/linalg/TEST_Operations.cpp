/**
 * @file    TEST_Operations.cpp
 * @author  Marvin Smith
 * @date    11/13/2023
*/
#include <gtest/gtest.h>

// Terminus Libraries
#include <terminus/math/linalg/Operations.hpp>

namespace tmx = tmns::math;

/****************************************/
/*      Test the nullspace method       */
/****************************************/
TEST( linalg_operations, Rank_and_Nullspace )
{
    // Square Matrix
    tmx::MatrixN<double> magic( 3, 3, { 8, 1, 6, 
                                        3, 5, 7,
                                        4, 9, 2 } );

    auto nullsp = tmx::linalg::nullspace( magic );
    ASSERT_FALSE( nullsp.has_error() );
    ASSERT_EQ( 0u, nullsp.value().cols() );
    ASSERT_EQ( 0u, nullsp.value().rows() );
    ASSERT_EQ( 3,  tmx::linalg::rank(magic) );  // It's square yo
    ASSERT_EQ( 0u, tmx::linalg::nullity(magic) );

    {
        // Common example of nullspace
        tmx::Matrix<double> cow( 2, 4 );
        cow(0,0) = -1; cow(0,1) = 1; cow(0,2) = 2; cow(0,3) = 4;
        cow(1,0) = 2;  cow(1,1) = 0; cow(1,2) = 1; cow(1,3) = -7;

        nullsp = tmx::linalg::nullspace(cow);
        ASSERT_EQ( nullsp.value().cols(), 2u );
        ASSERT_EQ( nullsp.value().rows(), 4u );

        tmx::VectorN<double> definition_check;
        definition_check = cow * tmx::select_col(nullsp.value(),0);
        for( uint32_t i = 0; i < definition_check.size(); i++ )
        {
            ASSERT_NEAR( 0, definition_check(i), 1e-8 );
        }
        definition_check = cow * tmx::select_col( nullsp.value(), 1 );
        for ( uint32_t i = 0; i < definition_check.size(); i++ )
        {
            ASSERT_NEAR( 0, definition_check(i), 1e-8 );
        }
        ASSERT_EQ( 2,  tmx::linalg::rank( cow ) );
        ASSERT_EQ( 2u, tmx::linalg::nullity( cow ) );
    }

    {
        // Data that threw an error in the past
        float monkey_data[63] = {
            148291,148852,187,537654,539688,678,793,796,1,
        1.60418e+06,958416,974,1.67335e+06,999744,1016,1647,984,1,
        1.09336e+06,835077,753,1.5972e+06,1.2199e+06,1100,1452,1109,1,
        371148,156024,394,303324,127512,322,942,396,1,
        94659,64357,139,234945,159735,345,681,463,1,
        945260,125330,755,192808,25564,154,1252,166,1,
        391777,206916,401,430857,227556,441,977,516,1
        };

        tmx::Matrix_Proxy<float> monkey( monkey_data, 7, 9 );
        auto nullsp3 = tmx::linalg::nullspace( monkey );
        ASSERT_EQ( 2u, nullsp3.value().cols() );
        ASSERT_EQ( 9u, nullsp3.value().rows() );
        ASSERT_EQ( 7, tmx::linalg::rank( monkey ) );
        ASSERT_EQ( 2u, tmx::linalg::nullity( monkey ) );

        tmx::VectorN<double> definition_check;
        definition_check = monkey * tmx::select_col( nullsp3.value(), 0 );
        for( uint32_t i = 0; i < definition_check.size(); i++ )
        {
            ASSERT_NEAR( 0, definition_check(i), 1e-1 );
        }
        definition_check = monkey * select_col( nullsp3.value(), 1 );
        for ( uint32_t i = 0; i < definition_check.size(); i++ )
        {
            ASSERT_NEAR( 0, definition_check(i), 1e-1 );
        }

        auto nullsp4 = tmx::linalg::nullspace( tmx::transpose( monkey ) );
        ASSERT_FALSE( nullsp4.has_error() );
        ASSERT_EQ( 0u, nullsp4.value().cols() );
        ASSERT_EQ( 0u, nullsp4.value().rows() );
        ASSERT_EQ(  7, tmx::linalg::rank( tmx::transpose( monkey ) ) );
        ASSERT_EQ( 0u, tmx::linalg::nullity( tmx::transpose( monkey ) ) );
    }

    {
        // More Data that threw an error in the past
        double shark_data[63] = {
            1.36483e+06,846250,1354,687456,426250,682,1008,625,1,
            1.28992e+06,750141,1323,607425,353241,623,975,567,1,
            1.2395e+06,710892,1302,572152,328146,601,952,546,1,
            1.26153e+06,792550,1310,636543,399905,661,963,605,1,
            1.23405e+06,740430,1299,593750,356250,625,950,570,1,
            1.22094e+06,799748,1292,637875,417825,675,945,619,1,
            1.15748e+06,708400,1265,562725,344400,615,915,560,1
        };

        tmx::Matrix_Proxy<double> shark( shark_data, 7, 9 );
        auto nullsp5 = tmx::linalg::nullspace(shark);
        ASSERT_EQ( nullsp5.value().cols(), 2u );
        ASSERT_EQ( nullsp5.value().rows(), 9u );

        tmx::VectorN<double> definition_check;
        definition_check = shark * tmx::select_col( nullsp5.value(), 0 );
        for( size_t i = 0; i < definition_check.size(); i++ )
        {
            ASSERT_NEAR( 0, definition_check(i), 1e-5 );
        }
        definition_check = shark * tmx::select_col( nullsp5.value(), 1 );
      
        for( size_t i = 0; i < definition_check.size(); i++ )
        {
          ASSERT_NEAR( 0, definition_check(i), 1e-5 );
        }

        ASSERT_EQ(  7, tmx::linalg::rank(shark) );
        ASSERT_EQ( 2u, tmx::linalg::nullity(shark) );
    }
}