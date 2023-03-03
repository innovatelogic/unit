cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/macros.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/modules/FindOpenCV.cmake)

set(CMAKE_SUPPRESS_REGENERATION true)

set(CMAKE_EXTRA_INCLUDES ${G_BIN_DIR}/../include/)

if (WIN32)
    set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH})
endif(WIN32)

# UT section
if (${ENABLE_TESTING})
set(GTEST_INCLUDE_DIRS ${GTEST_PATH}/include)
enable_testing()

include_directories(${MIPL_INCLUDE_DIR})
endif(${ENABLE_TESTING})

if(APPLE)
elseif(WIN32)
set(ADD_CXX_FLAGS_DEBUG "-DDEBUG")
set(ADD_CXX_FLAGS_REL "-DRELEASE -DNDEBUG")
endif()

SET (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${ADD_CXX_FLAGS_DEBUG}" CACHE STRING "cxx d compile flags" FORCE)
SET (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${ADD_CXX_FLAGS_REL}" CACHE STRING "cxx r compile flags" FORCE)

add_subdirectory(core)
