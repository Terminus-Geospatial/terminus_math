/**
 * @file    TEST_Matrix_Operations.cpp
 * @author  Marvin Smith
 * @date    10/12/2023
 */
#include <gtest/gtest.h>

#include <terminus/math/matrix.hpp>

namespace tmx = tmns::math;

class Matrix_Operations : public ::testing::Test
{
    protected:

        void SetUp() override
        {

        }

        // Create some test matrices
        tmx::Matrix<double,4,3> mat_01 { {  1,  2,  3,
                                            4,  5,  6,
                                            7,  8,  9,
                                           10, 11, 12 } };
        
        tmx::Matrix<double,3,4> mat_02 { {  1,  2,  3,  4,
                                            5,  6,  7,  8,
                                            9, 10, 11, 12 } };

};

/****************************************************/
/*          Test the Select Column Method           */
/****************************************************/
TEST_F( Matrix_Operations, select_col_test_01 )
{
    // Verify picking a column works
    {
        auto col_check = tmx::select_col( mat_01, 0 );
        ASSERT_EQ( col_check.size(), 4 );
        ASSERT_NEAR( col_check[0],  1, 0.001 );
        ASSERT_NEAR( col_check[1],  4, 0.001 );
        ASSERT_NEAR( col_check[2],  7, 0.001 );
        ASSERT_NEAR( col_check[3], 10, 0.001 );
        ASSERT_ANY_THROW( col_check[4] );
        ASSERT_ANY_THROW( col_check[30]  );
    }

    {
        const auto col_check = tmx::select_col( mat_01, 1 );
        ASSERT_EQ( col_check.size(), 4 );
        ASSERT_NEAR( col_check[0],  2, 0.001 );
        ASSERT_NEAR( col_check[1],  5, 0.001 );
        ASSERT_NEAR( col_check[2],  8, 0.001 );
        ASSERT_NEAR( col_check[3], 11, 0.001 );
        ASSERT_ANY_THROW( col_check[4] );
        ASSERT_ANY_THROW( col_check[30]  );
    }
    {
        auto col_check = tmx::select_col( mat_01, 2 );
        ASSERT_EQ( col_check.size(), 4 );
        ASSERT_NEAR( col_check[0],  3, 0.001 );
        ASSERT_NEAR( col_check[1],  6, 0.001 );
        ASSERT_NEAR( col_check[2],  9, 0.001 );
        ASSERT_NEAR( col_check[3], 12, 0.001 );
        ASSERT_ANY_THROW( col_check[4] );
        ASSERT_ANY_THROW( col_check[30]  );
    }

    // This should fail
    {
        auto col_check = tmx::select_col( mat_01, 3 );
        ASSERT_EQ( col_check.size(), 4 );
        ASSERT_ANY_THROW( col_check[0]  );
        ASSERT_ANY_THROW( col_check[1]  );
        ASSERT_ANY_THROW( col_check[2]  );
        ASSERT_ANY_THROW( col_check[3]  );
        ASSERT_ANY_THROW( col_check[4] );
        ASSERT_ANY_THROW( col_check[30]  );
    }

    // This should fail
    {
        auto col_check = tmx::select_col( mat_01, 30 );
        ASSERT_ANY_THROW( col_check[0] );
    }

    // Check mat 2
    {
        auto col_check = tmx::select_col( mat_02, 0 );
        ASSERT_EQ( col_check.size(), 3 );
        ASSERT_NEAR( col_check[0],  1, 0.001 );
        ASSERT_NEAR( col_check[1],  5, 0.001 );
        ASSERT_NEAR( col_check[2],  9, 0.001 );
        ASSERT_ANY_THROW( col_check[3] );
        ASSERT_ANY_THROW( col_check[30]  );
    }

    {
        auto col_check = tmx::select_col( mat_02, 1 );
        ASSERT_EQ( col_check.size(), 3 );
        ASSERT_NEAR( col_check[0],  2, 0.001 );
        ASSERT_NEAR( col_check[1],  6, 0.001 );
        ASSERT_NEAR( col_check[2], 10, 0.001 );
        ASSERT_ANY_THROW( col_check[3] );
        ASSERT_ANY_THROW( col_check[30]  );
    }
    {
        auto col_check = tmx::select_col( mat_02, 2 );
        ASSERT_EQ( col_check.size(), 3 );
        ASSERT_NEAR( col_check[0],  3, 0.001 );
        ASSERT_NEAR( col_check[1],  7, 0.001 );
        ASSERT_NEAR( col_check[2], 11, 0.001 );
        ASSERT_ANY_THROW( col_check[3] );
        ASSERT_ANY_THROW( col_check[30]  );
    }

    // This should fail
    {
        auto col_check = tmx::select_col( mat_02, 4 );
        ASSERT_EQ( col_check.size(), 3 );
        ASSERT_ANY_THROW( col_check[0]  );
        ASSERT_ANY_THROW( col_check[1]  );
        ASSERT_ANY_THROW( col_check[2]  );
        ASSERT_ANY_THROW( col_check[3]  );
        ASSERT_ANY_THROW( col_check[30]  );
    }

    // This should fail
    {
        auto col_check = tmx::select_col( mat_02, 30 );
        ASSERT_ANY_THROW( col_check[0] );
    }
}

/****************************************************/
/*          Test the Select Column Method           */
/****************************************************/
TEST_F( Matrix_Operations, select_col_test_02_transpose )
{
    // Tranpose
    auto mat_01_t = tmx::transpose( mat_01 );
    auto mat_02_t = tmx::transpose( mat_02 );

    // Verify picking a column works
    {
        auto col_check = tmx::select_col( mat_01_t, 0 );
        ASSERT_EQ( col_check.size(), 3 );
        ASSERT_NEAR( col_check[0],  1, 0.001 );
        ASSERT_NEAR( col_check[1],  2, 0.001 );
        ASSERT_NEAR( col_check[2],  3, 0.001 );
        ASSERT_ANY_THROW( col_check[3] );
        ASSERT_ANY_THROW( col_check[30]  );
    }

    {
        auto col_check = tmx::select_col( mat_01_t, 1 );
        ASSERT_EQ( col_check.size(), 3 );
        ASSERT_NEAR( col_check[0],  4, 0.001 );
        ASSERT_NEAR( col_check[1],  5, 0.001 );
        ASSERT_NEAR( col_check[2],  6, 0.001 );
        ASSERT_ANY_THROW( col_check[3] );
        ASSERT_ANY_THROW( col_check[30]  );
    }
    {
        auto col_check = tmx::select_col( mat_01_t, 2 );
        ASSERT_EQ( col_check.size(), 3 );
        ASSERT_NEAR( col_check[0],  7, 0.001 );
        ASSERT_NEAR( col_check[1],  8, 0.001 );
        ASSERT_NEAR( col_check[2],  9, 0.001 );
        ASSERT_ANY_THROW( col_check[3] );
        ASSERT_ANY_THROW( col_check[30]  );
    }
    {
        auto col_check = tmx::select_col( mat_01_t, 3 );
        ASSERT_EQ( col_check.size(), 3 );
        ASSERT_NEAR( col_check[0], 10, 0.001 );
        ASSERT_NEAR( col_check[1], 11, 0.001 );
        ASSERT_NEAR( col_check[2], 12, 0.001 );
        ASSERT_ANY_THROW( col_check[3] );
        ASSERT_ANY_THROW( col_check[30]  );
    }

    // This should fail
    {
        auto col_check = tmx::select_col( mat_01_t, 4 );
        ASSERT_EQ( col_check.size(), 3 );
        ASSERT_ANY_THROW( col_check[0]  );
        ASSERT_ANY_THROW( col_check[1]  );
        ASSERT_ANY_THROW( col_check[2]  );
        ASSERT_ANY_THROW( col_check[3]  );
        ASSERT_ANY_THROW( col_check[4]  );
        ASSERT_ANY_THROW( col_check[30]  );
    }

    // This should fail
    {
        auto col_check = tmx::select_col( mat_01_t, 30 );
        ASSERT_ANY_THROW( col_check[0] );
    }

    // Check mat 2
    {
        auto col_check = tmx::select_col( mat_02_t, 0 );
        ASSERT_EQ( col_check.size(), 4 );
        ASSERT_NEAR( col_check[0], 1, 0.001 );
        ASSERT_NEAR( col_check[1], 2, 0.001 );
        ASSERT_NEAR( col_check[2], 3, 0.001 );
        ASSERT_NEAR( col_check[3], 4, 0.001 );
        ASSERT_ANY_THROW( col_check[4] );
        ASSERT_ANY_THROW( col_check[30]  );
    }

    {
        auto col_check = tmx::select_col( mat_02_t, 1 );
        ASSERT_EQ( col_check.size(), 4 );
        ASSERT_NEAR( col_check[0], 5, 0.001 );
        ASSERT_NEAR( col_check[1], 6, 0.001 );
        ASSERT_NEAR( col_check[2], 7, 0.001 );
        ASSERT_NEAR( col_check[3], 8, 0.001 );
        ASSERT_ANY_THROW( col_check[4] );
        ASSERT_ANY_THROW( col_check[30]  );
    }
    {
        auto col_check = tmx::select_col( mat_02_t, 2 );
        ASSERT_EQ( col_check.size(), 4 );
        ASSERT_NEAR( col_check[0],  9, 0.001 );
        ASSERT_NEAR( col_check[1], 10, 0.001 );
        ASSERT_NEAR( col_check[2], 11, 0.001 );
        ASSERT_NEAR( col_check[3], 12, 0.001 );
        ASSERT_ANY_THROW( col_check[4] );
        ASSERT_ANY_THROW( col_check[30]  );
    }

    // This should fail
    {
        auto col_check = tmx::select_col( mat_02_t, 4 );
        ASSERT_EQ( col_check.size(), 4 );
        ASSERT_ANY_THROW( col_check[0]  );
        ASSERT_ANY_THROW( col_check[1]  );
        ASSERT_ANY_THROW( col_check[2]  );
        ASSERT_ANY_THROW( col_check[3]  );
        ASSERT_ANY_THROW( col_check[30]  );
    }

    // This should fail
    {
        auto col_check = tmx::select_col( mat_02_t, 30 );
        ASSERT_ANY_THROW( col_check[0] );
    }
}

/*************************************************/
/*          Test the Select Row Method           */
/*************************************************/
TEST_F( Matrix_Operations, select_row_test_01 )
{
    // Verify picking a column works
    {
        std::cout << mat_01.to_log_string() << std::endl;
        auto row_check = tmx::select_row( mat_01, 0 );
        std::cout << row_check.to_string() << std::endl;
        ASSERT_EQ( row_check.size(), 3 );
        ASSERT_NEAR( row_check[0], 1, 0.001 );
        ASSERT_NEAR( row_check[1], 2, 0.001 );
        ASSERT_NEAR( row_check[2], 3, 0.001 );
        ASSERT_ANY_THROW( row_check[3] );
        ASSERT_ANY_THROW( row_check[30]  );
    }

    {
        auto row_check = tmx::select_row( mat_01, 1 );
        ASSERT_EQ( row_check.size(), 3 );
        ASSERT_NEAR( row_check[0], 4, 0.001 );
        ASSERT_NEAR( row_check[1], 5, 0.001 );
        ASSERT_NEAR( row_check[2], 6, 0.001 );
        ASSERT_ANY_THROW( row_check[3] );
        ASSERT_ANY_THROW( row_check[30]  );
    }
    {
        auto row_check = tmx::select_row( mat_01, 2 );
        ASSERT_EQ( row_check.size(), 3 );
        ASSERT_NEAR( row_check[0], 7, 0.001 );
        ASSERT_NEAR( row_check[1], 8, 0.001 );
        ASSERT_NEAR( row_check[2], 9, 0.001 );
        ASSERT_ANY_THROW( row_check[3] );
        ASSERT_ANY_THROW( row_check[30]  );
    }
    {
        auto row_check = tmx::select_row( mat_01, 3 );
        ASSERT_EQ( row_check.size(), 3 );
        ASSERT_NEAR( row_check[0], 10, 0.001 );
        ASSERT_NEAR( row_check[1], 11, 0.001 );
        ASSERT_NEAR( row_check[2], 12, 0.001 );
        ASSERT_ANY_THROW( row_check[3] );
        ASSERT_ANY_THROW( row_check[30]  );
    }

    // This should fail
    {
        auto row_check = tmx::select_row( mat_01, 4 );
        ASSERT_EQ( row_check.size(), 3 );
        ASSERT_ANY_THROW( row_check[0]  );
        ASSERT_ANY_THROW( row_check[1]  );
        ASSERT_ANY_THROW( row_check[2]  );
        ASSERT_ANY_THROW( row_check[3]  );
        ASSERT_ANY_THROW( row_check[4] );
        ASSERT_ANY_THROW( row_check[30]  );
    }

    // This should fail
    {
        auto row_check = tmx::select_row( mat_01, 30 );
        ASSERT_ANY_THROW( row_check[0] );
    }

    // Check mat 2
    {
        auto row_check = tmx::select_row( mat_02, 0 );
        ASSERT_EQ( row_check.size(), 4 );
        ASSERT_NEAR( row_check[0],  1, 0.001 );
        ASSERT_NEAR( row_check[1],  2, 0.001 );
        ASSERT_NEAR( row_check[2],  3, 0.001 );
        ASSERT_NEAR( row_check[3],  4, 0.001 );
        ASSERT_ANY_THROW( row_check[4] );
        ASSERT_ANY_THROW( row_check[30]  );
    }

    {
        auto row_check = tmx::select_row( mat_02, 1 );
        ASSERT_EQ( row_check.size(), 4 );
        ASSERT_NEAR( row_check[0],  5, 0.001 );
        ASSERT_NEAR( row_check[1],  6, 0.001 );
        ASSERT_NEAR( row_check[2],  7, 0.001 );
        ASSERT_NEAR( row_check[3],  8, 0.001 );
        ASSERT_ANY_THROW( row_check[4] );
        ASSERT_ANY_THROW( row_check[30]  );
    }
    {
        auto row_check = tmx::select_row( mat_02, 2 );
        ASSERT_EQ( row_check.size(), 4 );
        ASSERT_NEAR( row_check[0],  9, 0.001 );
        ASSERT_NEAR( row_check[1], 10, 0.001 );
        ASSERT_NEAR( row_check[2], 11, 0.001 );
        ASSERT_NEAR( row_check[3], 12, 0.001 );
        ASSERT_ANY_THROW( row_check[4] );
        ASSERT_ANY_THROW( row_check[30]  );
    }

    // This should fail
    {
        auto row_check = tmx::select_row( mat_02, 3 );
        ASSERT_EQ( row_check.size(), 4 );
        ASSERT_ANY_THROW( row_check[0]  );
        ASSERT_ANY_THROW( row_check[1]  );
        ASSERT_ANY_THROW( row_check[2]  );
        ASSERT_ANY_THROW( row_check[3]  );
        ASSERT_ANY_THROW( row_check[30]  );
    }

    // This should fail
    {
        auto row_check = tmx::select_row( mat_02, 30 );
        ASSERT_ANY_THROW( row_check[0] );
    }
}

/*************************************************/
/*          Test the Select Row Method           */
/*************************************************/
TEST_F( Matrix_Operations, select_row_test_02_transpose )
{    
    // Tranpose
    auto mat_01_t = tmx::transpose( mat_01 );
    auto mat_02_t = tmx::transpose( mat_02 );

    // Verify picking a column works
    {
        auto row_check = tmx::select_row( mat_01_t, 0 );
        ASSERT_EQ( row_check.size(), 4 );
        ASSERT_NEAR( row_check[0],  1, 0.001 );
        ASSERT_NEAR( row_check[1],  4, 0.001 );
        ASSERT_NEAR( row_check[2],  7, 0.001 );
        ASSERT_NEAR( row_check[3], 10, 0.001 );
        ASSERT_ANY_THROW( row_check[4] );
        ASSERT_ANY_THROW( row_check[30]  );
    }

    {
        auto row_check = tmx::select_row( mat_01_t, 1 );
        ASSERT_EQ( row_check.size(), 4 );
        ASSERT_NEAR( row_check[0],  2, 0.001 );
        ASSERT_NEAR( row_check[1],  5, 0.001 );
        ASSERT_NEAR( row_check[2],  8, 0.001 );
        ASSERT_NEAR( row_check[3], 11, 0.001 );
        ASSERT_ANY_THROW( row_check[4] );
        ASSERT_ANY_THROW( row_check[30]  );
    }
    {
        auto row_check = tmx::select_row( mat_01_t, 2 );
        ASSERT_EQ( row_check.size(), 4 );
        ASSERT_NEAR( row_check[0],  3, 0.001 );
        ASSERT_NEAR( row_check[1],  6, 0.001 );
        ASSERT_NEAR( row_check[2],  9, 0.001 );
        ASSERT_NEAR( row_check[3], 12, 0.001 );
        ASSERT_ANY_THROW( row_check[4] );
        ASSERT_ANY_THROW( row_check[30]  );
    }

    // This should fail
    {
        auto row_check = tmx::select_row( mat_01_t, 4 );
        ASSERT_EQ( row_check.size(), 4 );
        ASSERT_ANY_THROW( row_check[0] );
        ASSERT_ANY_THROW( row_check[1] );
        ASSERT_ANY_THROW( row_check[2] );
        ASSERT_ANY_THROW( row_check[3] );
        ASSERT_ANY_THROW( row_check[30] );
    }

    // This should fail
    {
        auto row_check = tmx::select_row( mat_01_t, 30 );
        ASSERT_ANY_THROW( row_check[0] );
    }

    // Check mat 2
    {
        auto row_check = tmx::select_row( mat_02_t, 0 );
        ASSERT_EQ( row_check.size(), 3 );
        ASSERT_NEAR( row_check[0],  1, 0.001 );
        ASSERT_NEAR( row_check[1],  5, 0.001 );
        ASSERT_NEAR( row_check[2],  9, 0.001 );
        ASSERT_ANY_THROW( row_check[3] );
        ASSERT_ANY_THROW( row_check[30]  );
    }

    {
        auto row_check = tmx::select_row( mat_02_t, 1 );
        ASSERT_EQ( row_check.size(), 3 );
        ASSERT_NEAR( row_check[0],  2, 0.001 );
        ASSERT_NEAR( row_check[1],  6, 0.001 );
        ASSERT_NEAR( row_check[2], 10, 0.001 );
        ASSERT_ANY_THROW( row_check[3] );
        ASSERT_ANY_THROW( row_check[30]  );
    }
    {
        auto row_check = tmx::select_row( mat_02_t, 2 );
        ASSERT_EQ( row_check.size(), 3 );
        ASSERT_NEAR( row_check[0],  3, 0.001 );
        ASSERT_NEAR( row_check[1],  7, 0.001 );
        ASSERT_NEAR( row_check[2], 11, 0.001 );
        ASSERT_ANY_THROW( row_check[3] );
        ASSERT_ANY_THROW( row_check[30]  );
    }
    {
        auto row_check = tmx::select_row( mat_02_t, 3 );
        ASSERT_EQ( row_check.size(), 3 );
        ASSERT_NEAR( row_check[0],  4, 0.001 );
        ASSERT_NEAR( row_check[1],  8, 0.001 );
        ASSERT_NEAR( row_check[2], 12, 0.001 );
        ASSERT_ANY_THROW( row_check[3] );
        ASSERT_ANY_THROW( row_check[30]  );
    }

    // This should fail
    {
        auto row_check = tmx::select_row( mat_02_t, 4 );
        ASSERT_EQ( row_check.size(), 3 );
        ASSERT_ANY_THROW( row_check[0]  );
        ASSERT_ANY_THROW( row_check[1]  );
        ASSERT_ANY_THROW( row_check[2]  );
        ASSERT_ANY_THROW( row_check[3]  );
        ASSERT_ANY_THROW( row_check[30]  );
    }

    // This should fail
    {
        auto row_check = tmx::select_row( mat_02, 30 );
        ASSERT_ANY_THROW( row_check[0] );
    }
}

/************************************************/
/*      Test Weird Matrix Multiplication        */
/************************************************/
TEST_F( Matrix_Operations, matrix_multiplication_blended )
{
    // Create Proxy Matrix
    const tmx::Matrix_Proxy<const double,3,3> mat_p( mat_01.data() );

    const tmx::Matrix_Transpose mat_t( mat_02 );

    // Multiply matrices
    auto result = mat_p * mat_t;

    std::array<double,9> exp_result {  30,  36,  42,
                                       66,  81,  96,
                                      102, 126, 150 };

    size_t counter = 0;
    for( auto it = result.begin(); it != result.end(); it++ )
    {
        ASSERT_NEAR( (*it), exp_result[counter++], 0.01 );
    }
}

/************************************************/
/*      Test Weird Matrix Multiplication        */
/************************************************/
TEST_F( Matrix_Operations, matrix_multiplication_4_mats )
{
    std::vector<double> data { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    // Create duplicate matrices
    tmx::MatrixN<double> mat_1( 3, 3, data );
    tmx::MatrixN<double> mat_2( 3, 3, data );
    tmx::MatrixN<double> mat_3( 3, 3, data );
    tmx::MatrixN<double> mat_4( 3, 3, data );

    // Multiply matrices
    auto mat_5 = mat_1 * mat_2;
    auto mat_6 = mat_3 * mat_4;

    auto mat_7 = mat_5 * mat_6;

    std::array<double,9> exp_result {  7560.0000,   9288.0000,  11016.0000,
                                      17118.0000,  21033.0000,  24948.0000,
                                      26676.0000,  32778.0000,  38880.0000 };

    size_t counter = 0;
    for( auto it = mat_7.begin(); it != mat_7.end(); it++ )
    {
        ASSERT_NEAR( (*it), exp_result[counter++], 0.01 );
    }
}

/************************************************/
/*      Test Weird Matrix Multiplication        */
/************************************************/
TEST_F( Matrix_Operations, matrix_multiplication_vector_blended )
{
    // Create Proxy Matrix
    const tmx::Matrix_Proxy<const double,3,3> mat_p( mat_01.data() );

    const tmx::Matrix_Transpose mat_t( mat_02 );

    // Multiply matrices
    auto result = mat_p * mat_t;

    std::array<double,9> exp_result {  30,  36,  42,
                                       66,  81,  96,
                                      102, 126, 150 };

    size_t counter = 0;
    for( auto it = result.begin(); it != result.end(); it++ )
    {
        ASSERT_NEAR( (*it), exp_result[counter++], 0.01 );
    }
}
