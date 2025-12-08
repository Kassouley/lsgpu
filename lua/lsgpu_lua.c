#include <lua.h>
#include <lauxlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "lsgpu.h"

/**
 * LSGPU Lua Module
 * 
 * Provides Lua bindings for the LSGPU library to query GPU device information
 */

/* Metatable names */
#define LSGPU_GPU_LIST_META "lsgpu_gpu_list"

/* GPU List userdata structure */
typedef struct {
    lsgpu_gpu_list_t* list;
} lsgpu_gpu_list_ud;

/* ============================================================================
 * Lua API Functions
 * ============================================================================ */

/**
 * lsgpu.init() -> boolean
 * Initialize LSGPU library
 */
static int lua_lsgpu_init(lua_State *L) {
    int result = lsgpu_init();
    lua_pushboolean(L, result == 0);
    return 1;
}

/**
 * lsgpu.fini() -> boolean
 * Finalize LSGPU library
 */
static int lua_lsgpu_fini(lua_State *L) {
    int result = lsgpu_fini();
    lua_pushboolean(L, result == 0);
    return 1;
}

/**
 * lsgpu.lua_lsgpu_query_gpus() -> userdata or nil
 * Query available GPU devices and return as userdata
 * This preserves the original data for binary encoding/decoding
 */
static int lua_lsgpu_query_gpus(lua_State *L) {
    lsgpu_gpu_list_ud *ud = (lsgpu_gpu_list_ud *)lua_newuserdata(L, sizeof(lsgpu_gpu_list_ud));
    
    ud->list = (lsgpu_gpu_list_t *)malloc(sizeof(lsgpu_gpu_list_t));
    if (!ud->list) {
        lua_pushnil(L);
        lua_pushstring(L, "Memory allocation failed");
        return 2;
    }

    ud->list->count = 0;
    ud->list->entries = NULL;

    if (lsgpu_query_gpus_data(ud->list) != 0) {
        free(ud->list);
        ud->list = NULL;
        lua_pushnil(L);
        lua_pushstring(L, "Failed to query GPU devices");
        return 2;
    }

    luaL_getmetatable(L, LSGPU_GPU_LIST_META);
    lua_setmetatable(L, -2);
    return 1;
}

/**
 * lsgpu.write_binary(gpus, filename) -> boolean, string
 * Write GPU data to binary file
 * gpus: userdata from query_gpus_userdata()
 * filename: output file path
 */
static int lua_lsgpu_write_binary(lua_State *L) {
    lsgpu_gpu_list_ud *ud = (lsgpu_gpu_list_ud *)luaL_checkudata(L, 1, LSGPU_GPU_LIST_META);
    const char *filename = luaL_checkstring(L, 2);

    if (!ud->list) {
        lua_pushboolean(L, 0);
        lua_pushstring(L, "Invalid GPU list");
        return 2;
    }

    if (lsgpu_write_gpu_data_binary(ud->list, filename) != 0) {
        lua_pushboolean(L, 0);
        lua_pushstring(L, "Failed to write GPU data to binary file");
        return 2;
    }

    lua_pushboolean(L, 1);
    return 1;
}

/**
 * lsgpu.read_binary(filename) -> userdata or nil, string
 * Read GPU data from binary file
 * filename: input file path
 */
static int lua_lsgpu_read_binary(lua_State *L) {
    const char *filename = luaL_checkstring(L, 1);
    
    lsgpu_gpu_list_ud *ud = (lsgpu_gpu_list_ud *)lua_newuserdata(L, sizeof(lsgpu_gpu_list_ud));
    
    ud->list = (lsgpu_gpu_list_t *)malloc(sizeof(lsgpu_gpu_list_t));
    if (!ud->list) {
        lua_pushnil(L);
        lua_pushstring(L, "Memory allocation failed");
        return 2;
    }

    ud->list->count = 0;
    ud->list->entries = NULL;

    if (lsgpu_read_gpu_data_binary(ud->list, filename) != 0) {
        free(ud->list);
        ud->list = NULL;
        lua_pushnil(L);
        lua_pushstring(L, "Failed to read GPU data from binary file");
        return 2;
    }

    luaL_getmetatable(L, LSGPU_GPU_LIST_META);
    lua_setmetatable(L, -2);
    return 1;
}

/**
 * lsgpu.print(gpus) -> boolean, string
 * Print GPU data to sdtout
 * gpus: userdata from query_gpus_userdata()
 */
static int lua_lsgpu_print(lua_State *L) {
    lsgpu_gpu_list_ud *ud = (lsgpu_gpu_list_ud *)luaL_checkudata(L, 1, LSGPU_GPU_LIST_META);

    if (!ud->list) {
        lua_pushboolean(L, 0);
        lua_pushstring(L, "Invalid GPU list");
        return 2;
    }

    lsgpu_print_gpus_data(ud->list);
  
    lua_pushboolean(L, 1);
    return 1;
}

/**
 * GPU List userdata __gc metamethod
 * Automatically called when userdata is garbage collected
 */
static int lua_lsgpu_gpu_list_gc(lua_State *L) {
    lsgpu_gpu_list_ud *ud = (lsgpu_gpu_list_ud *)luaL_checkudata(L, 1, LSGPU_GPU_LIST_META);
    
    if (ud->list) {
        if (ud->list->entries) {
            free(ud->list->entries);
        }
        free(ud->list);
        ud->list = NULL;
    }

    return 0;
}

/**
 * GPU List userdata __len metamethod
 * Returns the number of GPU devices
 */
static int lua_lsgpu_gpu_list_len(lua_State *L) {
    lsgpu_gpu_list_ud *ud = (lsgpu_gpu_list_ud *)luaL_checkudata(L, 1, LSGPU_GPU_LIST_META);
    
    if (ud->list) {
        lua_pushinteger(L, ud->list->count);
    } else {
        lua_pushinteger(L, 0);
    }
    return 1;
}


/**
 * GPU List userdata __tostring metamethod
 */
static int lua_lsgpu_gpu_list_tostring(lua_State *L) {
    lsgpu_gpu_list_ud *ud = (lsgpu_gpu_list_ud *)luaL_checkudata(L, 1, LSGPU_GPU_LIST_META);
    
    if (ud->list) {
        lua_pushfstring(L, "lsgpu_gpu_list (%d devices)", ud->list->count);
    } else {
        lua_pushstring(L, "lsgpu_gpu_list (invalid)");
    }
    return 1;
}

/* ============================================================================
 * Module Definition
 * ============================================================================ */

static const struct luaL_Reg lsgpu_functions[] = {
    {"init",                  lua_lsgpu_init},
    {"fini",                  lua_lsgpu_fini},
    {"query_gpus",            lua_lsgpu_query_gpus},
    {"write_binary",          lua_lsgpu_write_binary},
    {"read_binary",           lua_lsgpu_read_binary},
    {"print",                 lua_lsgpu_print},
    {NULL,                    NULL}
};

static const struct luaL_Reg lsgpu_gpu_list_meta[] = {
    {"__gc",       lua_lsgpu_gpu_list_gc},
    {"__len",      lua_lsgpu_gpu_list_len},
    {"__tostring", lua_lsgpu_gpu_list_tostring},
    {NULL,         NULL}
};

/**
 * Lua module registration function
 * Called as: local lsgpu = require("lsgpu_lua")
 */
int luaopen_lsgpu_lua(lua_State *L) {
    /* Create the GPU list metatable */

    luaL_newmetatable(L, LSGPU_GPU_LIST_META);
    luaL_register(L, NULL, lsgpu_gpu_list_meta);
    lua_pop(L, 1);

    luaL_register(L, "lsgpu_lua", lsgpu_functions);
    return 1;
}