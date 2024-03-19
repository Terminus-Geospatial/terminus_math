#    File:    Find_Dependencies.cmake
#    Author:  Marvin Smith
#    Date:    10/14/2023
#

#--------------------#
#-      Eigen       -#
#--------------------#
find_package( Eigen3 REQUIRED )
message( STATUS "Eigen3_FOUND: ${Eigen3_FOUND}" )
message( STATUS "Eigen3_INCLUDE_DIRS: ${Eigen3_INCLUDE_DIRS}" )
include_directories( ${Eigen3_INCLUDE_DIRS} )

#--------------------#
#-      GDAL        -#
#--------------------#
find_package( GDAL CONFIG REQUIRED )
message( STATUS  "${COLOR_BOLD}GDAL_FOUND: ${COLOR_RESET} ${GDAL_FOUND}")

if( NOT GDAL_INCLUDE_DIRS )
    set( GDAL_INCLUDE_DIRS "/usr/local/include/gdal" )
endif()

message( "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}" )

if( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
    set( GDAL_INCLUDE_DIRS "/usr/local/include" )
else()
    set( GDAL_INCLUDE_DIRS "/usr/include/gdal" )
endif()
message( STATUS  "${COLOR_BOLD}GDAL_INCLUDE_DIRS:${COLOR_RESET}${GDAL_INCLUDE_DIRS}" )

#--------------------#
#-      Boost       -#
#--------------------#
set( Boost_USE_MULTITHREADED ON )
add_definitions( -DBOOST_ALL_DYN_LINK )
find_package( Boost REQUIRED )
#include_directories( ${Boost_INCLUDE_DIRS} )


#------------------------------------#
#-      Terminus Dependencies       -#
#------------------------------------#
find_package( terminus_core    REQUIRED )
find_package( terminus_log     REQUIRED )
find_package( terminus_outcome REQUIRED )

#----------------------------#
#-          OpenCV          -#
#----------------------------#
find_package( OpenCV REQUIRED )
message( STATUS "OpenCV_FOUND: ${OpenCV_FOUND}")
message( STATUS "OpenCV_INCLUDE_DIRS: ${OpenCV_INCLUDE_DIRS}")
message( STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")
include_directories( ${OpenCV_INCLUDE_DIRS} )