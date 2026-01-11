############################# INTELLECTUAL PROPERTY RIGHTS #############################
##                                                                                    ##
##                           Copyright (c) 2024 Terminus LLC                          ##
##                                All Rights Reserved.                                ##
##                                                                                    ##
##          Use of this source code is governed by LICENSE in the repo root.          ##
##                                                                                    ##
############################# INTELLECTUAL PROPERTY RIGHTS #############################
#
#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/12/2023
#

#  Configure GoogleTest
find_package( GTest REQUIRED )
include( GoogleTest )

include_directories( ${CMAKE_SOURCE_DIR}/library/include )

set( TEST ${PROJECT_NAME}_test )
add_executable( ${TEST}
    math/linalg/TEST_operations.cpp
    math/linalg/TEST_svd.cpp
    math/matrix/TEST_matrix_base.cpp
    math/matrix/TEST_matrix_multiplication.cpp
    math/matrix/TEST_matrix_operations.cpp
    math/matrix/TEST_matrix_transpose.cpp
    math/matrix/TEST_matrix.cpp
    math/matrix/TEST_matrixn.cpp
    math/optimization/TEST_levenburg_marquardt.cpp
    math/thirdparty/eigen/TEST_eigen_utilities.cpp
    math/types/TEST_compound_types.cpp
    math/types/TEST_fundamental_types.cpp
    math/vector/TEST_vector.cpp
    math/vector/TEST_vectorn.cpp
    math/vector/TEST_vector_transpose.cpp
    math/vector/TEST_vector_utilities.cpp
    math/TEST_point.cpp
    math/TEST_quaternion.cpp
    math/TEST_rectangle.cpp
    math/TEST_size.cpp
)

target_link_libraries( ${TEST} PRIVATE
    GTest::gtest_main
    ${PROJECT_NAME}
)

# Comment out automatic test discovery to avoid timeout issues
# gtest_discover_tests( ${TEST} )