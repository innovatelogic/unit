cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(unit_core)

#include(${CMAKE_CURRENT_SOURCE_DIR}/build/cmake/modules/FindOpenCV.cmake)

set(ENABLE_TESTING TRUE)

find_package(Python3 3.8...3.11.2 REQUIRED COMPONENTS Interpreter Development)

add_definitions(-DUNIT_CORE_LIB_COMPILE)

include_directories(${Python3_INCLUDE_DIR})

message( STATUS "Python3 found: " ${Python3_Development_FOUND} )
message( STATUS "Python3_INCLUDE_DIR: " ${Python3_INCLUDE_DIRS} )
message( STATUS "Python3_LIBRARIES: " ${Python3_LIBRARIES} )

if (LINUX)
set(OpenCV_DIR /mnt/d/Projects/opencv_build/opencv/build)
find_package(OpenCV REQUIRED PATHS "${CMAKE_SOURCE_DIR}/third_party/opencv")

find_package(MAVSDK REQUIRED)
set(MAVLINK_LIBS MAVSDK::mavsdk
    MAVSDK::mavsdk_action
    MAVSDK::mavsdk_telemetry
)


set(ONNX_RUNTIME_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third_party/onnxruntime/include)

set(ONNX_LIBS 
${CMAKE_SOURCE_DIR}/third_party/onnxruntime/Linux/libre2.a
${CMAKE_SOURCE_DIR}/third_party/onnxruntime/Linux/libnsync_cpp.a
${CMAKE_SOURCE_DIR}/third_party/onnxruntime/Linux/libonnx_proto.a
${CMAKE_SOURCE_DIR}/third_party/onnxruntime/Linux/libonnx.a
${CMAKE_SOURCE_DIR}/third_party/onnxruntime/Linux/libprotoc.a
${CMAKE_SOURCE_DIR}/third_party/onnxruntime/Linux/libprotobuf.a
${CMAKE_SOURCE_DIR}/third_party/onnxruntime/Linux/libonnxruntime.so.1.15.0
${CMAKE_SOURCE_DIR}/third_party/onnxruntime/Linux/libonnxruntime_session.a
${CMAKE_SOURCE_DIR}/third_party/onnxruntime/Linux/libonnxruntime_framework.a
${CMAKE_SOURCE_DIR}/third_party/onnxruntime/Linux/libonnxruntime_common.a
${CMAKE_SOURCE_DIR}/third_party/onnxruntime/Linux/libonnxruntime_graph.a
${CMAKE_SOURCE_DIR}/third_party/onnxruntime/Linux/libonnxruntime_util.a
${CMAKE_SOURCE_DIR}/third_party/onnxruntime/Linux/libonnxruntime_optimizer.a
${CMAKE_SOURCE_DIR}/third_party/onnxruntime/Linux/libonnxruntime_mlas.a
${CMAKE_SOURCE_DIR}/third_party/onnxruntime/Linux/libonnxruntime_providers.a
${CMAKE_SOURCE_DIR}/third_party/onnxruntime/Linux/libonnxruntime_test_utils.a
${CMAKE_SOURCE_DIR}/third_party/onnxruntime/Linux/libonnxruntime_flatbuffers.a
${CMAKE_SOURCE_DIR}/third_party/onnxruntime/Linux/libonnxruntime_mocked_allocator.a
)

set(GEOGRAPHYCLIB_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/third_party/georgraphiclib/include)
set(GEOGRAPHYCLIB_LIBS ${CMAKE_SOURCE_DIR}/third_party/georgraphiclib/lib/libGeographicLib.so.23.1.0)

endif()


include_directories(include)
include_directories(src)

set(SRC_INCLUDES
    include/platform.h
    include/core.h
    )

# Source files and headers section...
set(SRC_FILES
    src/core.cpp
    )

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Target creation section...
add_library(${PROJECT_NAME} SHARED ${SRC_INCLUDES} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE 
${CMAKE_SOURCE_DIR}/third_party
${ONNX_RUNTIME_INCLUDE_DIR}
${Python3_INCLUDE_DIRS}
${OpenCV_INCLUDE_DIRS}
${GEOGRAPHYCLIB_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} 
${Python3_LIBRARIES}
${MAVLINK_LIBS}
${OpenCV_LIBS}
${GEOGRAPHYCLIB_LIBS}
${ONNX_LIBS}
pthread
dl
)

set_target_properties(${PROJECT_NAME} PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${G_BIN_DIR}
                      LIBRARY_OUTPUT_DIRECTORY ${G_BIN_DIR}
                      ARCHIVE_OUTPUT_DIRECTORY ${G_BIN_DIR}
                      RUNTIME_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
                      RUNTIME_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR})

#target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

if (WIN32)
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".pyd")

elseif(LINUX)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-Wl,-rpath,./")
endif()

if (LINUX)
add_custom_command(TARGET ${PROJECT_NAME}
                    POST_BUILD
                    COMMAND ${CMAKE_SOURCE_DIR}/build/deploy_linux.sh
                    ${CMAKE_SOURCE_DIR}
                    ${G_BIN_DIR})
endif()

add_subdirectory(ut)

