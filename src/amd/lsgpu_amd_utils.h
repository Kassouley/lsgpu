#ifndef LSGPU_AMD_UTILS_H
#define LSGPU_AMD_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "lsgpu_type.h"
#include "amd/lsgpu_amd_type.h"

#define FOR_EACH_FIELD(macro) \
macro("",                      "Device Type",            device_type_t,    device_type) \
macro("  ",                    "Node",                   u32,              node) \
macro("  ",                    "Name",                   str64,            name) \
macro("  ",                    "Product Name",           str64,            product_name) \
macro("  ",                    "UUID",                   str21,            uuid) \
macro("  ",                    "Nearest CPU Node",       u32,              nearest_cpu_node) \
macro("  ",                    "Driver Node ID",         u32,              driver_node_id) \
macro("",                      "Architecture",           str64,            isa) \
macro("  ",                    "Feature",                feature_t,        feature) \
macro("  ",                    "Machine Model",          machine_model_t,  machine_model) \
macro("  ",                    "Version Major",          u16,              version_major) \
macro("  ",                    "Version Minor",          u16,              version_minor) \
macro("Memory:\n  ",           "Cache Size",             cache_t,          cache_size) \
macro("  ",                    "Cacheline Size",         u32,              cacheline_size) \
macro("  ",                    "Max Mem Clock",          u32,              max_mem_clock) \
macro("Execution Model:\n  ",  "Wavefront Size",         u32,              wavefront_size) \
macro("  ",                    "FBarrier Max Size",      u32,              fbarrier_max_size) \
macro("  ",                    "Workgroup Max Size",     u16,              workgrp_max_size) \
macro("  ",                    "Workgroup Max Dim",      u16_xyz_t,        workgrp_max_dim) \
macro("  ",                    "Grid Max Size",          u32,              grid_max_size) \
macro("  ",                    "Grid Max Dim",           u32_xyz_t,        grid_max_dim) \
macro("Queue Properties:\n  ", "Queue Type",             queue_type_t,     queue_type) \
macro("  ",                    "Queues Max",             u32,              queues_max) \
macro("  ",                    "Queue Min Size",         u32,              queue_min_size) \
macro("  ",                    "Queue Max Size",         u32,              queue_max_size) \
macro("Topology:\n  ",         "CU Count",               u32,              cu_count) \
macro("  ",                    "Max Wave Per CU",        u32,              max_wave_per_cu) \
macro("  ",                    "Num SIMD per CU",        u32,              num_simd_per_cu) \
macro("  ",                    "Num SE",                 u32,              num_se) \
macro("  ",                    "Num SA per SE",          u32,              num_sa_per_se) \
macro("  ",                    "Num SDMA Engines",       u32,              num_sdma_eng) \
macro("  ",                    "Num SDMA XGMI Engines",  u32,              num_sdma_xgmi_eng) \
macro("  ",                    "Num XCC",                u32,              num_xcc) \
macro("  ",                    "Chip ID",                u32,              chip_id) \
macro("",                      "Max Clock Freq. (MHz)",  u32,              max_clock_freq) \
macro("",                      "Driver UID",             u32,              driver_uid) \



struct lsgpu_gpu_data_s {
    #define SET_FIELD(_1, _2, type, name) type name;
    FOR_EACH_FIELD(SET_FIELD)
    #undef SET_FIELD
};


#endif // LSGPU_AMD_UTILS_H