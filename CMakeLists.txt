cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
    set(PLATFORM_ENV "linux")
    set(SHELL_EXT "sh")
    set(SHELL_RUN ${SHELL_EXT})
else()
    set(LINUX FALSE)
    if(APPLE)
        set(PLATFORM_ENV "xcode")
        set(SHELL_EXT "sh")
        set(SHELL_RUN ${SHELL_EXT})
    else()
        set(PLATFORM_ENV "msvs")
        set(SHELL_EXT cmd)
        set(SHELL_RUN ${SHELL_EXT} /C)
    endif()
endif()

project(unit VERSION "0.1")

#set(CMAKE_C_COMPILER "gcc-7")
#set(CMAKE_CXX_COMPILER "/usr/bin/g++-7")

set(ENABLE_TESTING TRUE)

include(CheckCXXCompilerFlag)
include(build/cmake/macros.cmake)

enable_language(C)
enable_language(CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

message(STATUS "CMAKE_STARTED:")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /EHa /MP /wd4996") #/WX
    set(add_debug_flags "/ZI /Od /fp:fast" CACHE STRING "" FORCE)
    set(add_release_flags "/Zi /Ox /Ob2 /Oi /Ot /Oy /GT /fp:fast" CACHE STRING "" FORCE)

    # avx compile time var
    if("${AVX}" STREQUAL "1")
        string(APPEND add_release_flags "/arch:AVX")
    elseif("${AVX}" STREQUAL "2")
        string(APPEND add_release_flags " /arch:AVX2")
    elseif("${AVX}" STREQUAL "3")
        string(APPEND add_release_flags " /arch:AVX2") # because msvc doesn't support avx 512 at the moment
    endif()    
    
    set(add_debug_linker_flags "/LTCG /DEBUG:FASTLINK /OPT:NOLBR")
    set(add_release_linker_flags "/LTCG /DEBUG:FASTLINK /OPT:NOLBR /OPT:REF") # /DEBUG:FULL for outside companies builds
endif()

if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
    set(CMAKE_C_FLAGS "${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)

    SET(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} ${add_release_linker_flags}"
        CACHE STRING "Flags used by the linker during Release build type." FORCE)
    SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} ${add_debug_linker_flags}"
        CACHE STRING "Flags used by the linker during Debug build type." FORCE)
    set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS_RELEASE} ${add_release_flags}"
        CACHE STRING "Flags used by the compiler during Release build type." FORCE)
    set(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} ${add_debug_flags}"
        CACHE STRING "Flags used by the compiler during Debug build type." FORCE)
endif()

message(STATUS "CMAKE_CXX_COMPILER_ID:" ${CMAKE_CXX_COMPILER_ID})
message(STATUS "CMAKE_GENERATOR_PLATFORM:" ${CMAKE_GENERATOR_PLATFORM})
message(STATUS "CMAKE_SYSTEM_NAME:" ${CMAKE_SYSTEM_NAME})

add_definitions(-DUNICODE -D_UNICODE)

if(NOT APPLE)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(CMAKE_DEBUG_POSTFIX _d)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/build/cmake/modules/)

if(APPLE)

elseif(WIN32)

add_definitions(-DWIN32)
add_definitions(-DWIN32_LEAN_AND_MEAN)
add_definitions(-D_USE_MATH_DEFINES)
add_definitions(-DNOMINMAX)
add_definitions(-DqWinOS=1)
add_definitions(-DqWinUniversal=1)
add_definitions(-D_CONSOLE)

add_definitions(-D_VISUALC_)
add_definitions(-DNeedFunctionPrototypes)
add_definitions(-D_AFXDLL)
add_definitions(-DRMT_DLL)
add_definitions(-D_HAS_AUTO_PTR_ETC=1)

# to avoid solution file change use fixed mipl guid
set(${PROJECT_NAME}_GUID_CMAKE "384A545C-A6F2-4833-9260-F0F1F2F3F4F5" CACHE INTERNAL "Project GUID")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")

elseif(LINUX)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Ofast -w -Wall -Wno-sign-compare -flax-vector-conversions -fpermissive -msse4.1 -Wno-unused-parameter -Wno-sign-compare -Wno-unused-function -Wno-unused-variable -Wno-error=unused-variable -Wno-error=unused-parameter -Wno-error=sign-compare -Wno-error=pointer-sign")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -w -Wall -ldl -fPIE -pie")
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -fPIE -pie")
#set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-Bsymbolic")
endif()

set(TARGET_FILENAME target_unit.cmake)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(G_BIN_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bin)


message( STATUS "CMAKE_COMMAND: " ${CMAKE_CURRENT_SOURCE_DIR} )
message( STATUS "EXECUTABLE_OUTPUT_PATH:" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
message( STATUS "CMAKE_COMMAND:" ${CMAKE_COMMAND} )
message( STATUS "CMAKE_SOURCE_DIR:" ${CMAKE_SOURCE_DIR})
message( STATUS "CMAKE_INSTALL_PREFIX:" ${CMAKE_INSTALL_PREFIX})
message( STATUS "CMAKE_PREFIX_PATH:" ${CMAKE_PREFIX_PATH})
message( STATUS "G_BIN_DIR: " ${G_BIN_DIR})
message( STATUS "COMPILER: " ${CMAKE_CXX_COMPILER_ID})
message( STATUS "TARGET:" ${TARGET_FILENAME})

add_definitions(-DPRODUCT_ID_${PRODUCT_ID})

file(MAKE_DIRECTORY ${G_BIN_DIR})

find_file(TARGET_INCLUDE
    ${TARGET_FILENAME}
    ${CMAKE_SOURCE_DIR}
    NO_DEFAULT_PATH)

if (TARGET_INCLUDE)
    message(STATUS "Found target include cmake: " ${TARGET_FILENAME})
    include(${TARGET_INCLUDE})
else()
    message(FATAL_ERROR "No target file:" ${TARGET_FILENAME})
endif()

