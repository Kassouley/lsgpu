cmake_minimum_required(VERSION 3.10)
project(lsgpu C)

#
# Vendor selection
#
set(LSGPU_VENDOR "AMD" CACHE STRING "GPU vendor: AMD or NVIDIA")
set_property(CACHE LSGPU_VENDOR PROPERTY STRINGS AMD NVIDIA)

#
# Require vendor include path
#
if (NOT DEFINED LSGPU_VENDOR_INCLUDE_DIR OR LSGPU_VENDOR_INCLUDE_DIR STREQUAL "")
    message(FATAL_ERROR
        "You must set LSGPU_VENDOR_INCLUDE_DIR with:\n"
        "  cmake -DLSGPU_VENDOR_INCLUDE_DIR=<vendor include dir>\n"
    )
endif()

#
# Select vendor-specific source file
#
if (LSGPU_VENDOR STREQUAL "AMD")
    set(VENDOR_SRC src/amd/lsgpu_amd.c)
elseif (LSGPU_VENDOR STREQUAL "NVIDIA")
    set(VENDOR_SRC src/nvidia/lsgpu_nvidia.c)
else()
    message(FATAL_ERROR "Invalid LSGPU_VENDOR. Must be AMD or NVIDIA.")
endif()

#
# Common sources
#
set(LSGPU_SOURCES src/lsgpu.c ${VENDOR_SRC})

#
# Create a static library with core lsgpu functionality
# This library can be reused by both the CLI and Lua module
#
add_library(lsgpu_core STATIC ${LSGPU_SOURCES})


set_property(TARGET lsgpu_core PROPERTY POSITION_INDEPENDENT_CODE ON)


#
# Configure the static library
#
target_include_directories(lsgpu_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${LSGPU_VENDOR_INCLUDE_DIR}
)

if (LSGPU_VENDOR STREQUAL "AMD")
    target_compile_definitions(lsgpu_core PUBLIC __LSGPU_VENDOR_AMD__)
    target_link_libraries(lsgpu_core PUBLIC hsa-runtime64)
elseif (LSGPU_VENDOR STREQUAL "NVIDIA")
    target_compile_definitions(lsgpu_core PUBLIC __LSGPU_VENDOR_NVIDIA__)
endif()


#
# Build options
#
option(LSGPU_BUILD_CLI "Build lsgpu command-line executable" ON)
option(LSGPU_BUILD_LUA "Build lsgpu Lua module" OFF)

#
# Build lsgpu CLI command if enabled
#
if (LSGPU_BUILD_CLI)
    add_executable(${PROJECT_NAME} src/lsgpu_cli.c)
    target_link_libraries(${PROJECT_NAME} PRIVATE lsgpu_core)
endif()

#
# Build Lua module if enabled
#
if (LSGPU_BUILD_LUA)
    add_subdirectory(lua)
endif()