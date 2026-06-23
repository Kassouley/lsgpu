cmake_minimum_required(VERSION 3.10)

project(lsgpu LANGUAGES C VERSION 1.0.0)

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
        "  cmake .. -DLSGPU_VENDOR_INCLUDE_DIR=<vendor include dir>\n"
    )
endif()

#
# Select vendor-specific sources
#
if (LSGPU_VENDOR STREQUAL "AMD")
    set(LSGPU_UTILS_SOURCES      src/amd/lsgpu_amd_utils.c)
    set(LSGPU_QUERY_SOURCES      src/amd/lsgpu_amd_query.c)
    set(LSGPU_COMPILE_DEFINITION __LSGPU_VENDOR_AMD__)
elseif (LSGPU_VENDOR STREQUAL "NVIDIA")
    set(LSGPU_COMPILE_DEFINITION __LSGPU_VENDOR_NVIDIA__)
    message(FATAL_ERROR "NVIDIA support is not yet implemented.")
else()
    message(FATAL_ERROR "Invalid LSGPU_VENDOR. Must be AMD or NVIDIA.")
endif()

#
# Common sources
#
set(LSGPU_SOURCES src/lsgpu.c)

add_library(lsgpu_utils STATIC ${LSGPU_SOURCES} ${LSGPU_UTILS_SOURCES})
target_include_directories(lsgpu_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
set_property(TARGET lsgpu_utils PROPERTY POSITION_INDEPENDENT_CODE ON)
target_compile_definitions(lsgpu_utils PUBLIC ${LSGPU_COMPILE_DEFINITION})


add_library(lsgpu_query STATIC ${LSGPU_QUERY_SOURCES})
target_include_directories(lsgpu_query PUBLIC 
    ${LSGPU_VENDOR_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
set_property(TARGET lsgpu_query PROPERTY POSITION_INDEPENDENT_CODE ON)
target_compile_definitions(lsgpu_query PUBLIC ${LSGPU_COMPILE_DEFINITION})

add_library(lsgpu_core INTERFACE)
target_link_libraries(lsgpu_core INTERFACE lsgpu_utils lsgpu_query)

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
    target_link_libraries(${PROJECT_NAME} PRIVATE lsgpu_query)
    target_link_libraries(${PROJECT_NAME} PRIVATE lsgpu_utils)
    
    if (LSGPU_VENDOR STREQUAL "AMD")
        target_link_libraries(${PROJECT_NAME} PRIVATE hsa-runtime64)
    elseif (LSGPU_VENDOR STREQUAL "NVIDIA")
        message(FATAL_ERROR "NVIDIA support is not yet implemented.")
    endif()
endif()

#
# Build Lua module if enabled
#
if (LSGPU_BUILD_LUA)
    add_subdirectory(lua)
endif()