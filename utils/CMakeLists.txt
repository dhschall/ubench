
## Common Utils Library

set(SOURCES
    configs.cc
)

add_library(utils ${SOURCES})


# ## Libaries ##################################
# ## Add boost library
# find_package(Boost COMPONENTS program_options REQUIRED)

# include_directories( ${Boost_INCLUDE_DIRS} )
# target_link_libraries(utils ${Boost_LIBRARIES})



## Add YAML library
include(FetchContent)

FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG 0.8.0 # Can be a tag (yaml-cpp-x.x.x), a commit hash, or a branch name (master)
)
FetchContent_MakeAvailable(yaml-cpp)

target_link_libraries(utils yaml-cpp::yaml-cpp)




#### M5Ops library


if(${ARCH} MATCHES aarch64)
    ## Add ARM64 specific library
    set(M5_SOURCES
        m5lib/arm/m5op.S
    )
elseif(${ARCH} MATCHES riscv64)
    ## Add RISC-V specific library
    set(M5_SOURCES
        m5lib/riscv/m5op.S
    )
else()
    ## Add x86_64 specific library
    set(M5_SOURCES
        m5lib/x86/m5op.S
    )
endif()

add_library(m5lib ${M5_SOURCES})

