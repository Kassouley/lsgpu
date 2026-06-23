#ifndef LSGPU_AMD_QUERY_H
#define LSGPU_AMD_QUERY_H

#include <stdio.h>
#include <stdlib.h>

#include "amd/hsa_api.h"

#define QUERY(INFO, var) hsa_agent_get_info(agent, (hsa_agent_info_t) INFO, var)

#define FOR_EACH_INFO(macro) \
macro(device_type,       HSA_AGENT_INFO_DEVICE) \
macro(node,              HSA_AGENT_INFO_NODE) \
macro(name,              HSA_AGENT_INFO_NAME) \
macro(product_name,      HSA_AMD_AGENT_INFO_PRODUCT_NAME) \
macro(uuid,              HSA_AMD_AGENT_INFO_UUID) \
macro(nearest_cpu_node,  -1) \
macro(driver_node_id,    HSA_AMD_AGENT_INFO_DRIVER_NODE_ID) \
macro(isa,               -1) \
macro(feature,           HSA_AGENT_INFO_FEATURE) \
macro(machine_model,     HSA_AGENT_INFO_MACHINE_MODEL) \
macro(version_major,     HSA_AGENT_INFO_VERSION_MAJOR) \
macro(version_minor,     HSA_AGENT_INFO_VERSION_MINOR) \
macro(cache_size,        HSA_AGENT_INFO_CACHE_SIZE) \
macro(cacheline_size,    HSA_AMD_AGENT_INFO_CACHELINE_SIZE) \
macro(max_mem_clock,     HSA_AMD_AGENT_INFO_MEMORY_MAX_FREQUENCY) \
macro(wavefront_size,    HSA_AGENT_INFO_WAVEFRONT_SIZE) \
macro(fbarrier_max_size, HSA_AGENT_INFO_FBARRIER_MAX_SIZE) \
macro(workgrp_max_size,  HSA_AGENT_INFO_WORKGROUP_MAX_SIZE) \
macro(workgrp_max_dim,   HSA_AGENT_INFO_WORKGROUP_MAX_DIM) \
macro(grid_max_size,     HSA_AGENT_INFO_GRID_MAX_SIZE) \
macro(grid_max_dim,      HSA_AGENT_INFO_GRID_MAX_DIM) \
macro(queue_type,        HSA_AGENT_INFO_QUEUE_TYPE) \
macro(queues_max,        HSA_AGENT_INFO_QUEUES_MAX) \
macro(queue_min_size,    HSA_AGENT_INFO_QUEUE_MIN_SIZE) \
macro(queue_max_size,    HSA_AGENT_INFO_QUEUE_MAX_SIZE) \
macro(cu_count,          HSA_AMD_AGENT_INFO_COMPUTE_UNIT_COUNT) \
macro(max_wave_per_cu,   HSA_AMD_AGENT_INFO_MAX_WAVES_PER_CU) \
macro(num_simd_per_cu,   HSA_AMD_AGENT_INFO_NUM_SIMDS_PER_CU) \
macro(num_se,            HSA_AMD_AGENT_INFO_NUM_SHADER_ENGINES) \
macro(num_sa_per_se,     HSA_AMD_AGENT_INFO_NUM_SHADER_ARRAYS_PER_SE) \
macro(num_sdma_eng,      HSA_AMD_AGENT_INFO_NUM_SDMA_ENG) \
macro(num_sdma_xgmi_eng, HSA_AMD_AGENT_INFO_NUM_SDMA_XGMI_ENG) \
macro(num_xcc,           HSA_AMD_AGENT_INFO_NUM_XCC) \
macro(chip_id,           HSA_AMD_AGENT_INFO_CHIP_ID) \
macro(max_clock_freq,    HSA_AMD_AGENT_INFO_MAX_CLOCK_FREQUENCY) \
macro(driver_uid,        HSA_AMD_AGENT_INFO_DRIVER_UID) \


static inline const char* hsa_err_string(hsa_status_t status)
{
    const char* msg = NULL;
    if (hsa_status_string(status, &msg) != HSA_STATUS_SUCCESS)
        return "<unknown HSA status>";
    return msg ? msg : "<null>";
}

#define CHECK_HSA(call) do { \
    hsa_status_t _s = (call); \
    if (_s != HSA_STATUS_SUCCESS) { \
        fprintf(stderr, \
            "[lsgpu] %s:%d: %s failed: %s (%d)\n", \
            __FILE__, __LINE__, #call, hsa_err_string(_s), _s); \
        return -1; \
    } \
} while (0)



#endif // LSGPU_AMD_QUERY_H