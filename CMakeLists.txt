
cmake_minimum_required(VERSION 3.0.0)
project(ubench VERSION 0.1.0 LANGUAGES C CXX ASM)


set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g")

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

SET(BASEPATH ${CMAKE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${BASEPATH})

if(NOT DEFINED ARCH)
    set(ARCH x86)
endif()

## Own code ###################################
add_subdirectory(utils)
add_subdirectory(benchmarks)


## Create executable
set(TARGET_NAME ubench)
set(SOURCES
    main.cc
    )

add_executable(${TARGET_NAME} ${SOURCES})

target_link_libraries(${TARGET_NAME}
                       benchmarks
                       utils
                       m5lib
                       )
target_link_options(${TARGET_NAME} PRIVATE -Wl,--whole-archive $<TARGET_FILE:benchmarks> -Wl,--no-whole-archive)
target_compile_options(${TARGET_NAME} PRIVATE -Wall)




