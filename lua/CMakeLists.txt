cmake_minimum_required(VERSION 3.16)

project(lsgpu_lua C)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ---- Dependencies ----
if (NOT DEFINED LUA_INCLUDE_DIR)
    message(FATAL_ERROR "LUA_INCLUDE_DIR must be provided to build the ${PROJECT_NAME} module")
endif()

# ---- Source ----
set(SOURCE
    "${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.c"
)

# ---- Build the Lua module ----
add_library(${PROJECT_NAME} MODULE ${SOURCE})

set_target_properties(${PROJECT_NAME} PROPERTIES
    PREFIX "" 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/lua 
    OUTPUT_NAME "${PROJECT_NAME}"
)

# ---- Include dirs ----
target_include_directories(${PROJECT_NAME} PRIVATE
    ${LUA_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

# ---- Link to the lsgpu static library ----
if (NOT TARGET lsgpu_core)
    message(FATAL_ERROR "lsgpu_core target not found. Ensure the parent lsgpu CMakeLists.txt is included.")
endif()

set_property(TARGET lsgpu_core PROPERTY POSITION_INDEPENDENT_CODE ON)
target_link_libraries(${PROJECT_NAME} PRIVATE lsgpu_core)
