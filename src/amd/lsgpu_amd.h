#ifndef LSGPU_AMD_H
#define LSGPU_AMD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <hsa/hsa.h>
#include <hsa/hsa_ext_amd.h>

static inline const char* get_device_type(uint32_t id) {
    static const char* device_types[] = { "CPU", "GPU", "DSP", "AIE" };
    uint32_t count = sizeof(device_types) / sizeof(device_types[0]);
    return (id < count) ? device_types[id] : "-";
}

static inline const char* get_feature_type(uint32_t id) {
    static const char* feature_types[] = { "AGENT DISPATCH", "KERNEL DISPATCH" };
    uint32_t count = sizeof(feature_types) / sizeof(feature_types[0]);
    return (id < count) ? feature_types[id] : "-";
}

static inline const char* get_machine_model_type(uint32_t id) {
    static const char* machine_model_types[] = { "SMALL", "LARGE" };
    uint32_t count = sizeof(machine_model_types) / sizeof(machine_model_types[0]);
    return (id < count) ? machine_model_types[id] : "-";
}

static inline const char* get_queue_type(uint32_t id) {
    static const char* queue_types[] = { "MULTI", "SINGLE", "COOPERATIVE" };
    uint32_t count = sizeof(queue_types) / sizeof(queue_types[0]);
    return (id < count) ? queue_types[id] : "-";
}

#include "lsgpu_type.h"
#include "lsgpu_amd_type.h"
#include "lsgpu_hsa_helper.h"

#define FOR_EACH_FIELD(macro) \
macro("",                      "Device Type",            device_type_t,    device_type,       HSA_AGENT_INFO_DEVICE) \
macro("  ",                    "Name",                   str64,            name,              HSA_AGENT_INFO_NAME) \
macro("  ",                    "Product Name",           str64,            product_name,      HSA_AMD_AGENT_INFO_PRODUCT_NAME) \
macro("  ",                    "UUID",                   str21,            uuid,              HSA_AMD_AGENT_INFO_UUID) \
macro("  ",                    "Node",                   u32,              node,              HSA_AGENT_INFO_NODE) \
macro("  ",                    "Nearest CPU Node",       u32,              nearest_cpu_node,  -1) \
macro("  ",                    "Driver Node ID",         u32,              driver_node_id,    HSA_AMD_AGENT_INFO_DRIVER_NODE_ID) \
macro("",                      "Architecture",           str,              isa,               -1) \
macro("  ",                    "Feature",                feature_t,        feature,           HSA_AGENT_INFO_FEATURE) \
macro("  ",                    "Machine Model",          machine_model_t,  machine_model,     HSA_AGENT_INFO_MACHINE_MODEL) \
macro("  ",                    "Version Major",          u16,              version_major,     HSA_AGENT_INFO_VERSION_MAJOR) \
macro("  ",                    "Version Minor",          u16,              version_minor,     HSA_AGENT_INFO_VERSION_MINOR) \
macro("Memory:\n  ",           "Cache Size",             cache_t,          cache_size,        HSA_AGENT_INFO_CACHE_SIZE) \
macro("  ",                    "Cacheline Size",         u32,              cacheline_size,    HSA_AMD_AGENT_INFO_CACHELINE_SIZE) \
macro("  ",                    "Max Mem Clock",          u32,              max_mem_clock,     HSA_AMD_AGENT_INFO_MEMORY_MAX_FREQUENCY) \
macro("Execution Model:\n  ",  "Wavefront Size",         u32,              wavefront_size,    HSA_AGENT_INFO_WAVEFRONT_SIZE) \
macro("  ",                    "FBarrier Max Size",      u32,              fbarrier_max_size, HSA_AGENT_INFO_FBARRIER_MAX_SIZE) \
macro("  ",                    "Workgroup Max Size",     u16,              workgrp_max_size,  HSA_AGENT_INFO_WORKGROUP_MAX_SIZE) \
macro("  ",                    "Workgroup Max Dim",      u16_xyz_t,        workgrp_max_dim,   HSA_AGENT_INFO_WORKGROUP_MAX_DIM) \
macro("  ",                    "Grid Max Size",          u32,              grid_max_size,     HSA_AGENT_INFO_GRID_MAX_SIZE) \
macro("  ",                    "Grid Max Dim",           u32_xyz_t,        grid_max_dim,      HSA_AGENT_INFO_GRID_MAX_DIM) \
macro("Queue Properties:\n  ", "Queue Type",             queue_type_t,     queue_type,        HSA_AGENT_INFO_QUEUE_TYPE) \
macro("  ",                    "Queues Max",             u32,              queues_max,        HSA_AGENT_INFO_QUEUES_MAX) \
macro("  ",                    "Queue Min Size",         u32,              queue_min_size,    HSA_AGENT_INFO_QUEUE_MIN_SIZE) \
macro("  ",                    "Queue Max Size",         u32,              queue_max_size,    HSA_AGENT_INFO_QUEUE_MAX_SIZE) \
macro("Topology\n  ",          "CU Count",               u32,              cu_count,          HSA_AMD_AGENT_INFO_COMPUTE_UNIT_COUNT) \
macro("  ",                    "Max Wave Per CU",        u32,              max_wave_per_cu,   HSA_AMD_AGENT_INFO_MAX_WAVES_PER_CU) \
macro("  ",                    "Num SIMD per CU",        u32,              num_simd_per_cu,   HSA_AMD_AGENT_INFO_NUM_SIMDS_PER_CU) \
macro("  ",                    "Num SE",                 u32,              num_se,            HSA_AMD_AGENT_INFO_NUM_SHADER_ENGINES) \
macro("  ",                    "Num SA per SE",          u32,              num_sa_per_se,     HSA_AMD_AGENT_INFO_NUM_SHADER_ARRAYS_PER_SE) \
macro("  ",                    "Num SDMA Engines",       u32,              num_sdma_eng,      HSA_AMD_AGENT_INFO_NUM_SDMA_ENG) \
macro("  ",                    "Num SDMA XGMI Engines",  u32,              num_sdma_xgmi_eng, HSA_AMD_AGENT_INFO_NUM_SDMA_XGMI_ENG) \
macro("  ",                    "Num XCC",                u32,              num_xcc,           HSA_AMD_AGENT_INFO_NUM_XCC) \
macro("  ",                    "Chip ID",                u32,              chip_id,           HSA_AMD_AGENT_INFO_CHIP_ID) \
macro("",                      "Max Clock Freq. (MHz)",  u32,              max_clock_freq,    HSA_AMD_AGENT_INFO_MAX_CLOCK_FREQUENCY) \
macro("",                      "Driver UID",             u32,              driver_uid,        HSA_AMD_AGENT_INFO_DRIVER_UID) \



typedef struct lsgpu_gpu_data_s {
    #define SET_FIELD(___, _, type, name, __) type name;
    FOR_EACH_FIELD(SET_FIELD)
    #undef SET_FIELD
} lsgpu_gpu_data_t;



#endif // LSGPU_AMD_H