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
#    Purpose:  Build terminus-math API

include_directories( include )

add_library( ${PROJECT_NAME} SHARED
                src/geo/axis.cpp
                src/geo/bounds.cpp
                src/geo/conversions.cpp
                src/geo/coord/ecef.cpp
                src/geo/coord/geographic.cpp
                src/geo/coord/utm.cpp
                src/geo/crs.cpp
                src/geo/datum.cpp
                src/geo/transform.cpp
                src/math/linalg/solvers.cpp
                src/math/linalg/svd.cpp
                src/math/quaternion.cpp
                src/math/quaternion_utilities.cpp )

target_link_libraries( ${PROJECT_NAME} PUBLIC
    Boost::headers
    Boost::log
    Boost::log_setup
    terminus_log::terminus_log
    terminus_outcome::terminus_outcome
    GDAL::GDAL
    ${OpenCV_LIBS}
)