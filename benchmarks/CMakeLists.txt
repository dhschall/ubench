
## Benchmark library

set(SOURCES
    base.cc
    template/simple_loop.cc
    branch/random.cc
    branch/throughput.cc

    btb/btb_stress.cc
    btb/btb_stress_asm.cc
)

add_library(benchmarks STATIC ${SOURCES})

target_include_directories(benchmarks PUBLIC "${BASEPATH}")
#target_link_libraries(benchmarks PUBLIC utils)

if(${ARCH} MATCHES aarch64)
    target_compile_definitions(benchmarks PUBLIC ARCH=2)
elseif(${ARCH} MATCHES riscv64)
    target_compile_definitions(benchmarks PUBLIC ARCH=3)
else()
    target_compile_definitions(benchmarks PUBLIC ARCH=1)
endif()



## Add YAML library
include(FetchContent)

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG 0.8.0 # Can be a tag (yaml-cpp-x.x.x), a commit hash, or a branch name (master)
)
FetchContent_MakeAvailable(yaml-cpp)

target_link_libraries(benchmarks yaml-cpp::yaml-cpp)


add_custom_target(
  gen_btb ALL
  COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/btb/btb_stress_gen.py ${CMAKE_CURRENT_SOURCE_DIR}/btb/btb_stress_gen.h
  BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/btb/btb_stress_gen.h
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/btb/btb_stress_gen.py
  COMMENT "Generating btb_stress_gen.h"
)

add_dependencies(benchmarks gen_btb)

add_custom_target(
  gen_btb_asm ALL
  COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/btb/btb_stress_asm_gen.py ${CMAKE_CURRENT_SOURCE_DIR}/btb/btb_stress_asm_gen.h
  BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/btb/btb_stress_asm_gen.h
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/btb/btb_stress_asm_gen.py
  COMMENT "Generating btb_stress_asm_gen.h"
)

add_dependencies(benchmarks gen_btb_asm)