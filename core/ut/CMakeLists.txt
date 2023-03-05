cmake_minimum_required(VERSION 3.7.0 FATAL_ERROR)

if (${ENABLE_TESTING})

set(PROJECT_TEST_NAME unittest_${PROJECT_NAME})

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/src/ut)

set(UT_SOURCE
        unit_ut.cpp
        main.cpp
)

# setup folder filters
#assign_source_group(${UT_SOURCE})

add_executable(${PROJECT_TEST_NAME} ${UT_SOURCE})

target_link_libraries(${PROJECT_TEST_NAME} ${PROJECT_NAME} ${ONNX_LIBS} gtest pthread dl)

add_definitions(-DUSE_MOCK)

add_test(NAME ${PROJECT_NAME_U}_TEST COMMAND ${PROJECT_TEST_NAME})

set_target_properties(${PROJECT_TEST_NAME} PROPERTIES FOLDER "UT" )

set_target_properties(${PROJECT_TEST_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${G_BIN_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${G_BIN_DIR}
        ARCHIVE_OUTPUT_DIRECTORY ${G_BIN_DIR}

        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${G_BIN_DIR}
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${G_BIN_DIR}
)

set_target_properties( ${PROJECT_TEST_NAME} PROPERTIES
        OUTPUT_NAME ${PROJECT_TEST_NAME}.test
        DEBUG_POSTFIX ""
)

endif(${ENABLE_TESTING})
