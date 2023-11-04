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

#  Give Vscode a fighting chance
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#  Set C++ 20 Support
set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

include(terminus_cmake)

#-------------------------------------#
#-     Add Manual CMake Scripts      -#
#-------------------------------------#
LIST( APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

#  Look for dependencies
include( "${PROJECT_SOURCE_DIR}/cmake/scripts/Find_Dependencies.cmake" )

add_subdirectory( src )

include( GNUInstallDirs )

install( TARGETS
               ${PROJECT_NAME}
         EXPORT
               ${PROJECT_NAME}Targets
         ARCHIVE
               DESTINATION
                    ${CMAKE_INSTALL_LIBDIR}
                    COMPONENT lib
          RUNTIME
               DESTINATION ${CMAKE_INSTALL_BINDIR}
               COMPONENT bin
          LIBRARY 
               DESTINATION ${CMAKE_INSTALL_LIBDIR}
               COMPONENT lib 
)

set( INSTALL_CMAKEDIR share/cmake/${PROJECT_NAME} )

install( EXPORT 
               ${PROJECT_NAME}Targets
         NAMESPACE
               "${PROJECT_NAME}::"
         DESTINATION
               ${INSTALL_CMAKEDIR}
)

message( "VERSION_FROM_CONANFILE: ${VERSION_FROM_CONANFILE}" )
message( "VERSION: ${PROJECT_VERSION}" )
message( "CMAKE_PROJECT_VERSION: ${CMAKE_PROJECT_VERSION}" )
include( CMakePackageConfigHelpers )
write_basic_package_version_file( 
     ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
     VERSION ${PROJECT_VERSION}
     COMPATIBILITY SameMajorVersion
)

configure_package_config_file( 
     ${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
     ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
     INSTALL_DESTINATION ${INSTALL_CMAKEDIR}
)

install(  FILES
               ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
          DESTINATION
               ${INSTALL_CMAKEDIR}
)

#  Enable code coverage
if( TERMINUS_MATH_ENABLE_COVERAGE )
     terminus_coverage_enable()
endif()

if( TERMINUS_MATH_ENABLE_TESTS )
     enable_testing()
     add_subdirectory( test/unit )
endif()

#  Copy all header files in the project folder
install( DIRECTORY 
               ${CMAKE_SOURCE_DIR}/include 
         DESTINATION
               ${CMAKE_INSTALL_INCLUDEDIR} )
