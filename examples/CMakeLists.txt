# ##############################################################################
# BUILD MACRO
# ##############################################################################
find_package(Threads)
file(GLOB EXAMPLES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
foreach(EXAMPLE ${EXAMPLES})
  get_filename_component(EXAMPLE_EXE ${EXAMPLE} NAME_WE)
  add_executable(${EXAMPLE_EXE} ${EXAMPLE})
  target_link_libraries(${EXAMPLE_EXE} ${CMAKE_THREAD_LIBS_INIT})
  target_link_libraries(${EXAMPLE_EXE} usque)
endforeach()
