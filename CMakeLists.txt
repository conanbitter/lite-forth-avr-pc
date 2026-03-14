cmake_minimum_required(VERSION 3.16)

project(lfa VERSION 0.1 LANGUAGES C)

set(EXECUTABLE_OUTPUT_PATH ./${CMAKE_BUILD_TYPE})

set(INCLUDE_FILES
    include.h
)

set(SRC_FILES
    main.c
)

list(TRANSFORM INCLUDE_FILES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/include/)
list(TRANSFORM SRC_FILES PREPEND ${CMAKE_CURRENT_SOURCE_DIR}/src/)

add_executable(lfa ${INCLUDE_FILES} ${SRC_FILES})
target_include_directories(lfa PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

#target_link_options(lfa PRIVATE -SUBSYSTEM:WINDOWS)

#if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
#    set_property(TARGET lfa PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded")
#endif()