cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(unit_core)

set(ENABLE_TESTING TRUE)

find_package(Python3 3.8...3.11.2 REQUIRED COMPONENTS Interpreter Development)

add_definitions(-DUNIT_CORE_LIB_COMPILE)

include_directories(${Python3_INCLUDE_DIR})

message( STATUS "Python3 found: " ${Python3_Development_FOUND} )
message( STATUS "Python3_INCLUDE_DIR: " ${Python3_INCLUDE_DIRS} )
message( STATUS "Python3_LIBRARIES: " ${Python3_LIBRARIES} )

if (LINUX)
set("OpenCV_DIR" /mnt/d/Projects/opencv_build/opencv/build)
find_package(OpenCV REQUIRED)

find_package(MAVSDK REQUIRED)
set(MAVLINK_LIBS MAVSDK::mavsdk
    MAVSDK::mavsdk_action
    MAVSDK::mavsdk_telemetry
)
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
${Python3_INCLUDE_DIRS}
${OpenCV_INCLUDE_DIRS}
 )

target_link_libraries(${PROJECT_NAME} 
${Python3_LIBRARIES}
${MAVLINK_LIBS}
${OpenCV_LIBS}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${G_BIN_DIR}
                      LIBRARY_OUTPUT_DIRECTORY ${G_BIN_DIR}
                      ARCHIVE_OUTPUT_DIRECTORY ${G_BIN_DIR}

                      RUNTIME_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
                      RUNTIME_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
                      )


#target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

if (WIN32)
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".pyd")

elseif(LINUX)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
endif()

add_custom_command(TARGET ${PROJECT_NAME}
                    POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    ${CMAKE_SOURCE_DIR}/pylib/pycore_test.py
                    ${G_BIN_DIR}/pycore_test.py
                    )

file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${G_BIN_DIR})

add_subdirectory(ut)

