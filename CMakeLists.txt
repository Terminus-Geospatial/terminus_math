#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/12/2023
#
#    Purpose:  Build terminus-math API

#  Set CMake Version
cmake_minimum_required( VERSION 3.22 FATAL_ERROR )

#  Configure Conan Search Paths
include( "${CMAKE_BINARY_DIR}/conan_toolchain.cmake" )
set( CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE )

# Set the Project-Name
project( ${NAME_FROM_CONANFILE}
            VERSION ${VERSION_FROM_CONANFILE}
            HOMEPAGE_URL ${URL_FROM_CONANFILE}
            DESCRIPTION ${DESCRIPTION_FROM_CONANFILE}
            LANGUAGES CXX
)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#  Set C++ 20 Support
set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

#  Give Vscode a fighting chance
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#  Setup libdl for debug GCC builds
if( CMAKE_BUILD_TYPE STREQUAL "debug" OR CMAKE_BUILD_TYPE STREQUAL "Debug" )
    if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic -ldl" )
    endif()
endif()

# Bring in CMake helpers for TERMINUS projects
include( terminus_cmake )

#-------------------------------------#
#-     Add Manual CMake Scripts      -#
#-------------------------------------#
LIST( APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

#  Look for dependencies
include( "${PROJECT_SOURCE_DIR}/cmake/scripts/Find_Dependencies.cmake" )

include_directories( include )

add_library( ${PROJECT_NAME} SHARED
                src/coordinate/Datum.cpp
                src/math/linalg/Solvers.cpp
                src/math/linalg/SVD.cpp
                src/math/Quaternion.cpp
                src/math/Quaternion_Utilities.cpp )

target_link_libraries( ${PROJECT_NAME} PUBLIC
    Boost::headers
Boost::log
    Boost::log_setup
    Boost::stacktrace
    terminus_core::terminus_core
    terminus_log::terminus_log
    terminus_outcome::terminus_outcome
    GDAL::GDAL
)

install( TARGETS ${PROJECT_NAME} )

terminus_lib_configure( ${PROJECT_NAME} )

#  Enable code coverage
if( TERMINUS_MATH_ENABLE_COVERAGE )
     terminus_coverage_enable()
endif()

if( TERMINUS_MATH_ENABLE_TESTS )
     enable_testing()
     add_subdirectory( test/unit )
endif()

install( DIRECTORY include/terminus DESTINATION include)