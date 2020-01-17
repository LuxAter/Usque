# ##############################################################################
# PROJECT
# ##############################################################################
cmake_minimum_required(VERSION 3.11.0 FATAL_ERROR)
project(
  Usque
  VERSION 0.1.0
  LANGUAGES CXX)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  set(USQUE_MAIN_PROJECT TRUE)
endif()

# ##############################################################################
# OPTIONS
# ##############################################################################
option(USQUE_BUILD_EXAMPLES "Compile included examples" ${USQUE_MAIN_PROJECT})

# ##############################################################################
# BUILD MODE
# ##############################################################################
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE
      "Debug"
      CACHE STRING "Build Mode" FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
  if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json")
    execute_process(
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
              ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json)
  endif()
endif()

# ##############################################################################
# STATIC ANALYSIS
# ##############################################################################
if(STATIC_ANALYSIS OR CMAKE_BUILD_TYPE STREQUAL "Release")
  find_program(
    CLANG_TIDY_EXE
    NAMES "clang-tidy"
    DOC "Path to clang tidy exe")
  if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
  endif()
  find_program(CPPCHECK_EXE NAME "cppcheck" DOC "Path to cppcheck exe")
  if(CPPCHECK_EXE)
    set(CMAKE_CXX_CPPCHECK "${CPPCHECK_EXE}" "--force")
  endif()
  find_program(CPPLINT_EXE "cpplint")
  if(CPPLINT_EXE)
    set(CMAKE_CXX_CPPLINT "${CPPLINT_EXE}")
  endif()
endif()

# ##############################################################################
# PROFILER
# ##############################################################################
include(WriteCompilerDetectionHeader)
set(USQUE_COMPILERS MSVC GNU Clang Intel AppleClang)
write_compiler_detection_header(
  FILE
  ${CMAKE_CURRENT_SOURCE_DIR}/include/usque/compiler.hpp
  PREFIX
  USQUE
  COMPILERS
  ${USQUE_COMPILERS}
  FEATURES
  cxx_constexpr
  cxx_deleted_functions
  cxx_func_identifier
  cxx_thread_local)
add_library(usque INTERFACE)
target_include_directories(usque INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/)

# ##############################################################################
# EXAMPLES
# ##############################################################################
if(USQUE_BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()
