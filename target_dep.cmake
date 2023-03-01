cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(dep NONE)

include(ExternalProject)
include(CMakeParseArguments)

#assign_source_group(dep)

ExternalProject_Add(googletest
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/utils/googletest
	DOWNLOAD_COMMAND ""
    # cmake arguments
    CMAKE_ARGS
			#-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
			   -BUILD_GMOCK=ON
			   -BUILD_GTEST=ON
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=${G_BIN_DIR}
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=${G_BIN_DIR}
			   -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG:PATH=${G_BIN_DIR}
			   -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE:PATH=${G_BIN_DIR}
			   -DCMAKE_DEBUG_POSTFIX=_d
			   -DBUILD_SHARED_LIBS=ON
			   -DGTEST_CREATE_SHARED_LIBRARY=1
			${ARGS_CMAKE_ARGS}# Disable install step
    INSTALL_COMMAND ""

    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    )	

    #add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/utils/googletest")

#add_custom_target(googletest-download)
#set_target_properties(googletest-download PROPERTIES FOLDER "UT" )