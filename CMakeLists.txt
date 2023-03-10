cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

function(get_all_targets _result _dir)
    get_property(_subdirs DIRECTORY "${_dir}" PROPERTY SUBDIRECTORIES)
    foreach(_subdir IN LISTS _subdirs)
        get_all_targets(${_result} "${_subdir}")
    endforeach()
    get_property(_sub_targets DIRECTORY "${_dir}" PROPERTY BUILDSYSTEM_TARGETS)
    set(${_result} ${${_result}} ${_sub_targets} PARENT_SCOPE)
endfunction()

function(add_subdirectory_with_folder _folder_name _folder)
    add_subdirectory(${_folder} ${ARGN})
    get_all_targets(_targets "${_folder}")
    foreach(_target IN LISTS _targets)
        set_target_properties(
            ${_target}
            PROPERTIES FOLDER "${_folder_name}"
        )
    endforeach()
endfunction()

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

set(ENABLE_TESTING TRUE)

#include(CheckCXXCompilerFlag)
#include(build/cmake/macros.cmake)

enable_language(C)
enable_language(CXX)

#set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY USE_FOLDERS TRUE)

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
add_definitions(-D_CONSOLE)
add_definitions(-D_VISUALC_)
add_definitions(-DNeedFunctionPrototypes)
add_definitions(-D_AFXDLL)
add_definitions(-DRMT_DLL)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")

elseif(LINUX)

if(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_INCLUDE_SYSTEM_FLAG_C "-isystem ")
endif()
#set(CMAKE_POSITION_INDEPENDENT_CODE ON)
#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Ofast -w -Wall -Wno-sign-compare -flax-vector-conversions -fpermissive -msse4.1 -Wno-unused-parameter -Wno-sign-compare -Wno-unused-function -Wno-unused-variable -Wno-error=unused-variable -Wno-error=unused-parameter -Wno-error=sign-compare -Wno-error=pointer-sign")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast -w -Wall -ldl ") #-fPIE -pie
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS}") # -fPIE -pie
#set(CMAKE_CXX_LINK_EXECUTABLE "{$CMAKE_CXX_LINK_EXECUTABLE} -R")
endif()

set(TARGET_FILENAME target_unit.cmake)
set(TARGET_DEP_FILENAME target_unit.cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(G_BIN_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/bin)
set(G_LIB_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lib)

message( STATUS "CMAKE_COMMAND: " ${CMAKE_CURRENT_SOURCE_DIR} )
message( STATUS "EXECUTABLE_OUTPUT_PATH:" ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
message( STATUS "CMAKE_COMMAND:" ${CMAKE_COMMAND} )
message( STATUS "CMAKE_SOURCE_DIR:" ${CMAKE_SOURCE_DIR})
message( STATUS "CMAKE_INSTALL_PREFIX:" ${CMAKE_INSTALL_PREFIX})
message( STATUS "CMAKE_PREFIX_PATH:" ${CMAKE_PREFIX_PATH})
message( STATUS "G_BIN_DIR: " ${G_BIN_DIR})
message( STATUS "COMPILER: " ${CMAKE_CXX_COMPILER_ID})
message( STATUS "TARGET:" ${TARGET_FILENAME})

file(MAKE_DIRECTORY ${G_BIN_DIR})
file(MAKE_DIRECTORY ${G_LIB_DIR})

find_file(TARGET_DEP_INCLUDE ${TARGET_FILENAME} ${CMAKE_SOURCE_DIR} NO_DEFAULT_PATH)
find_file(TARGET_INCLUDE ${TARGET_FILENAME} ${CMAKE_SOURCE_DIR} NO_DEFAULT_PATH)

if (TARGET_INCLUDE)
    message(STATUS "Found target dep include cmake: " ${TARGET_DEP_INCLUDE})
    message(STATUS "Found target include cmake: " ${TARGET_FILENAME})

    configure_file(target_dep.cmake dep/CMakeLists.txt)

    execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" . WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/dep")
    #execute_process(COMMAND "${CMAKE_COMMAND}" --build . WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/dep")

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    add_subdirectory_with_folder(dep "${CMAKE_CURRENT_SOURCE_DIR}/utils/googletest" EXCLUDE_FROM_ALL)
 
    include(${TARGET_INCLUDE})
else()
    message(FATAL_ERROR "No target file:" ${TARGET_FILENAME})
endif()

