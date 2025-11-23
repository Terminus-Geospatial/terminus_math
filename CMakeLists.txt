#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    7/12/2023
#
#    Purpose:  Build terminus-math API

#  Set CMake Version
cmake_minimum_required( VERSION 4.0.0 FATAL_ERROR )

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

#  Set C++ 23 Support
set( CMAKE_CXX_STANDARD 23 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

#  Give Vscode a fighting chance
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Bring in CMake helpers for TERMINUS projects
include( terminus_cmake )

# Force the high warning preset published by terminus_cmake across this project
set( TERMINUS_CXX_FLAGS ${TERMINUS_CXX_FLAGS_HIGH} CACHE STRING "Terminus warning preset" FORCE )

#  Treat warnings as errors
set( CMAKE_COMPILE_WARNING_AS_ERROR ON )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror" )

#-------------------------------------#
#-     Add Manual CMake Scripts      -#
#-------------------------------------#
LIST( APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

#  Look for dependencies
include( "${PROJECT_SOURCE_DIR}/cmake/scripts/Find_Dependencies.cmake" )

add_subdirectory( library )

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