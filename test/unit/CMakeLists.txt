#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/12/2023
#

#  Configure GoogleTest
find_package( GTest REQUIRED )
include( GoogleTest )

include_directories( ${CMAKE_SOURCE_DIR}/include )

set( TEST ${PROJECT_NAME}_test )
add_executable( ${TEST}
    coordinate/vw/TEST_Point_Transformations.cpp
    math/matrix/TEST_Matrix_Base.cpp
    math/matrix/TEST_Matrix_Multiplication.cpp
    math/matrix/TEST_Matrix_Operations.cpp
    math/matrix/TEST_Matrix_Transpose.cpp
    math/matrix/TEST_Matrix.cpp
    math/matrix/TEST_MatrixN.cpp
    math/optimization/TEST_Levenburg_Marquardt.cpp
    math/vector/TEST_Vector.cpp
    math/vector/TEST_VectorN.cpp
    math/vector/TEST_Vector_Transpose.cpp
    math/vector/TEST_Vector_Utilities.cpp
    math/TEST_Point.cpp
    math/TEST_Quaternion.cpp
    math/TEST_Rectangle.cpp
    math/TEST_Size.cpp
)

target_link_libraries( ${TEST} PRIVATE
    GTest::gtest_main
    ${PROJECT_NAME}
)

gtest_discover_tests( ${TEST} )