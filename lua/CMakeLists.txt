cmake_minimum_required(VERSION 3.10)

set(LSGPU_LUA lsgpu_lua)

# ---- Dependencies ----
if (NOT DEFINED LUA_INCLUDE_DIR)
    message(FATAL_ERROR "LUA_INCLUDE_DIR must be provided to build the ${LSGPU_LUA} module")
endif()

# ---- Source ----
set(SOURCE
    "${CMAKE_CURRENT_SOURCE_DIR}/${LSGPU_LUA}.c"
)

# ---- Build the Lua module ----
add_library(${LSGPU_LUA} MODULE ${SOURCE})

set_property(TARGET ${LSGPU_LUA} PROPERTY POSITION_INDEPENDENT_CODE ON)

set_target_properties(${LSGPU_LUA} PROPERTIES
    PREFIX "" 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/lua 
    OUTPUT_NAME "${LSGPU_LUA}"
)

# ---- Include dirs ----
target_include_directories(${LSGPU_LUA} PRIVATE
    ${LUA_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

# ---- Link to the lsgpu static library ----
if (NOT TARGET lsgpu_core)
    message(FATAL_ERROR "lsgpu_core target not found. Ensure the parent lsgpu CMakeLists.txt is included.")
endif()

target_link_libraries(${LSGPU_LUA} PRIVATE lsgpu_core)

if (LSGPU_VENDOR STREQUAL "AMD")
    target_link_libraries(${LSGPU_LUA} PRIVATE hsa-runtime64)
elseif (LSGPU_VENDOR STREQUAL "NVIDIA")
endif()
