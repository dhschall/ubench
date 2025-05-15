
## Benchmark library

set(SOURCES
    base.cc
    template/simple_loop.cc
)

add_library(benchmarks STATIC ${SOURCES})

target_include_directories(benchmarks PUBLIC "${BASEPATH}")
#target_link_libraries(benchmarks PUBLIC utils)


## Add YAML library
include(FetchContent)

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG 0.8.0 # Can be a tag (yaml-cpp-x.x.x), a commit hash, or a branch name (master)
)
FetchContent_MakeAvailable(yaml-cpp)

target_link_libraries(benchmarks yaml-cpp::yaml-cpp)