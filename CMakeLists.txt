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
set(SOURCES
    src/lsgpu.c
    ${VENDOR_SRC}
)

#
# Build executable
#
add_executable(${PROJECT_NAME} ${SOURCES})

#
# Include directories
#
target_include_directories(${PROJECT_NAME} PRIVATE ${LSGPU_VENDOR_INCLUDE_DIR} src)

#
# Vendor-specific macros & libraries
#
if (LSGPU_VENDOR STREQUAL "AMD")
    target_compile_definitions( ${PROJECT_NAME} PRIVATE __LSGPU_VENDOR_AMD__)
    target_link_libraries(      ${PROJECT_NAME} PRIVATE hsa-runtime64)
elseif (LSGPU_VENDOR STREQUAL "NVIDIA")
    target_compile_definitions( ${PROJECT_NAME} PRIVATE __LSGPU_VENDOR_NVIDIA__)
endif()
