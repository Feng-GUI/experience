cmake_minimum_required(VERSION 2.8.12)
PROJECT(fortnite-justdance-filter)
math(EXPR BITS "8*${CMAKE_SIZEOF_VOID_P}")

################################################################################
# Configuration
################################################################################

# OpenCV
SET(PATH_OPENCV "" CACHE PATH "OpenCV Source Code Directory")
if(PATH_OPENCV STREQUAL "")
	message(FATAL_ERROR "PATH_OPENCV not set!")
	return()
endif()
if(NOT EXISTS "${PATH_OPENCV}/include/opencv2")
	message(FATAL_ERROR "PATH_OPENCV is invalid!")
	return()
endif()

SET (PATH_OPENCV_BUILD "${PATH_OPENCV}/../build/")

SET(OpenCV_VERSION "342" CACHE STRING "Version of OpenCV to use.")
SET(OpenCV_LIBS
	debug "${PATH_OPENCV_BUILD}/lib/Debug/opencv_calib3d${OpenCV_VERSION}d.lib"
	optimized "${PATH_OPENCV_BUILD}/lib/Release/opencv_calib3d${OpenCV_VERSION}.lib"
	debug "${PATH_OPENCV_BUILD}/lib/Debug/opencv_core${OpenCV_VERSION}d.lib"
	optimized "${PATH_OPENCV_BUILD}/lib/Release/opencv_core${OpenCV_VERSION}.lib"
	debug "${PATH_OPENCV_BUILD}/lib/Debug/opencv_highgui${OpenCV_VERSION}d.lib"
	optimized "${PATH_OPENCV_BUILD}/lib/Release/opencv_highgui${OpenCV_VERSION}.lib"
	debug "${PATH_OPENCV_BUILD}/lib/Debug/opencv_imgcodecs${OpenCV_VERSION}d.lib"
	optimized "${PATH_OPENCV_BUILD}/lib/Release/opencv_imgcodecs${OpenCV_VERSION}.lib"
	debug "${PATH_OPENCV_BUILD}/lib/Debug/opencv_imgproc${OpenCV_VERSION}d.lib"
	optimized "${PATH_OPENCV_BUILD}/lib/Release/opencv_imgproc${OpenCV_VERSION}.lib"
	debug "${PATH_OPENCV_BUILD}/lib/Debug/opencv_dnn${OpenCV_VERSION}d.lib"
	optimized "${PATH_OPENCV_BUILD}/lib/Release/opencv_dnn${OpenCV_VERSION}.lib"
	debug "${PATH_OPENCV_BUILD}/3rdparty/lib/Debug/libpngd.lib"
	optimized "${PATH_OPENCV_BUILD}/3rdparty/lib/Release/libpng.lib"
	debug "${PATH_OPENCV_BUILD}/3rdparty/lib/Debug/zlibd.lib"
	optimized "${PATH_OPENCV_BUILD}/3rdparty/lib/Release/zlib.lib"
	debug "${PATH_OPENCV_BUILD}/3rdparty/lib/Debug/ittnotifyd.lib"
	optimized "${PATH_OPENCV_BUILD}/3rdparty/lib/Release/ittnotify.lib"
	debug "${PATH_OPENCV_BUILD}/3rdparty/lib/Debug/ippiwd.lib"
	optimized "${PATH_OPENCV_BUILD}/3rdparty/lib/Release/ippiw.lib"
)
include_directories("${PATH_OPENCV}/include")

# zlib
include_directories("${PATH_OPENCV}/3rdparty/include")

# intel IPP
#include_directories("${PATH_OPENCV}/3rdparty/ippicv/ippicv_win_331/include")
#include_directories("${PATH_OPENCV}/3rdparty/ippicv/ippiw_win_331/include")

# OBS Studio
SET(PATH_OBS_STUDIO "" CACHE PATH "OBS Studio Source Code Directory")
if(PATH_OBS_STUDIO STREQUAL "")
	message(FATAL_ERROR "PATH_OBS_STUDIO not set!")
	return()
endif()
if(NOT EXISTS "${PATH_OBS_STUDIO}/libobs/obs-module.h")
	message(FATAL_ERROR "PATH_OBS_STUDIO is invalid!")
	return()
endif()
SET(obsPath "${PATH_OBS_STUDIO}")
INCLUDE("${PATH_OBS_STUDIO}/cmake/external/Findlibobs.cmake")

# Distribute format
OPTION(BUILD_FOR_PUBLIC "Build for public release" OFF)
if(BUILD_FOR_PUBLIC)
	add_definitions(-DPUBLIC_RELEASE)
endif()


# Distribute format
OPTION(BUILD_SLOBS "Build for Streamlabs OBS" OFF)
if(BUILD_SLOBS)
	SET(DISTRIBUTE_PATH "${PROJECT_BINARY_DIR}/distribute/slobs")
	SET(DISTRIBUTE_PATH_BIN "obs-plugins")
	add_definitions(-DSLOBS_BUILD)
else()
	SET(DISTRIBUTE_PATH "${PROJECT_BINARY_DIR}/distribute/obs")
	SET(DISTRIBUTE_PATH_BIN "obs-plugins/${BITS}bit")
	add_definitions(-DOBS_STUDIO_BUILD)
endif()

################################################################################
# Setup
################################################################################

# Version
SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 1)
SET(VERSION_PATCH 1)

# Code
SET(fortnite-justdance-filter_HEADERS
	"${PROJECT_SOURCE_DIR}/FortniteFilter.h"
	"${PROJECT_SOURCE_DIR}/plugin.h"
)
SET(fortnite-justdance-filter_SOURCES
	"${PROJECT_SOURCE_DIR}/FortniteFilter.cpp"
	"${PROJECT_SOURCE_DIR}/plugin.cpp"
)
SET(fortnite-justdance-filter_LIBRARIES
	${OpenCV_LIBS}
	avrt.lib
	Shlwapi.lib
)

INCLUDE_DIRECTORIES(
	"${PROJECT_BINARY_DIR}"
	"${PROJECT_SOURCE_DIR}"
	"${PATH_OBS_STUDIO}/"
)

source_group("Plugin\\Headers" FILES ${fortnite-justdance-filter_HEADERS})
source_group("Plugin\\Sources" FILES ${fortnite-justdance-filter_SOURCES})

# Compiler Configuration
add_definitions(-D_CRT_SECURE_NO_WARNINGS) # Hide Microsofts insecurities
## All Warnings, Extra Warnings, Pedantic
if (MSVC)
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

################################################################################
# Building
################################################################################
ADD_LIBRARY(fortnite-justdance-filter MODULE
	${fortnite-justdance-filter_HEADERS}
	${fortnite-justdance-filter_SOURCES}
	${fortnite-justdance-filter_DATA}
)
TARGET_LINK_LIBRARIES(fortnite-justdance-filter
	${LIBOBS_LIBRARIES}
	${fortnite-justdance-filter_LIBRARIES}
)

################################################################################
# Installation / Distribution
################################################################################


add_custom_command(TARGET fortnite-justdance-filter POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${PROJECT_SOURCE_DIR}/data"
	"${DISTRIBUTE_PATH}/$<CONFIG>/data/obs-plugins/fortnite-justdance-filter"
)
add_custom_command(TARGET fortnite-justdance-filter POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	"$<TARGET_FILE:fortnite-justdance-filter>"
	"${DISTRIBUTE_PATH}/$<CONFIG>/${DISTRIBUTE_PATH_BIN}/$<TARGET_FILE_NAME:fortnite-justdance-filter>"
)
add_custom_command(TARGET fortnite-justdance-filter POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
	"$<TARGET_FILE_DIR:fortnite-justdance-filter>/fortnite-justdance-filter.pdb"
	"${DISTRIBUTE_PATH}/$<CONFIG>/${DISTRIBUTE_PATH_BIN}/fortnite-justdance-filter.pdb"
)
