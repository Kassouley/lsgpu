#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

#include "lsgpu.h"
#include "lsgpu_type.h"
#include "amd/lsgpu_amd_type.h"
#include "amd/lsgpu_amd_query.h"
#include "amd/lsgpu_amd_utils.h"
#include "amd/hsa_api.h"

static inline uint32_t get_node_id(hsa_agent_t agent) {
    uint32_t node;
    hsa_agent_get_info(agent, HSA_AGENT_INFO_NODE, &node);
    return node;
}

static inline uint32_t get_nearest_cpu_node(hsa_agent_t agent) {
    hsa_agent_t cpu;
    hsa_agent_get_info(agent, HSA_AMD_AGENT_INFO_NEAREST_CPU, &cpu);
    return get_node_id(cpu);
}

static inline char* get_isa_name(hsa_agent_t agent) {
    hsa_isa_t isa;
    hsa_agent_get_info(agent, HSA_AGENT_INFO_ISA, &isa);
    char* name = NULL;
    uint32_t len = 0;
    hsa_isa_get_info_alt(isa, HSA_ISA_INFO_NAME_LENGTH, &len);
    if (len != 0) {
        name = (char*) calloc(len, 1);
        hsa_isa_get_info_alt(isa, HSA_ISA_INFO_NAME, name);
    }
    return name;
}

static inline bool agent_is_gpu(hsa_agent_t agent) {
    uint32_t type = 0;
    hsa_agent_get_info(agent, HSA_AGENT_INFO_DEVICE, &type);
    return type == 1;
}

static hsa_status_t query_agent_data(hsa_agent_t agent, void* data) 
{
    if (!agent_is_gpu(agent)) return HSA_STATUS_SUCCESS;

    lsgpu_gpu_list_t *agents_list = (lsgpu_gpu_list_t*)data;
    struct lsgpu_gpu_data_s* agent_data = NULL;
    lsgpu_create_gpu_data(&agent_data);

    agents_list->entries[agents_list->count++] = agent_data;

    #define QUERY_FIELD(field, info) \
        if (info != -1) hsa_agent_get_info(agent, (hsa_agent_info_t) info, &agent_data->field);
    FOR_EACH_INFO(QUERY_FIELD)
    #undef QUERY_FIELD

    agent_data->nearest_cpu_node = get_nearest_cpu_node(agent);
    char* isa_name = get_isa_name(agent);

    strncpy(agent_data->isa, isa_name, sizeof(agent_data->isa) - 1);
    agent_data->isa[sizeof(agent_data->isa) - 1] = '\0';
    free(isa_name);

    return HSA_STATUS_SUCCESS;
}

static hsa_status_t count_agents(hsa_agent_t agent, void* data) {
    (void)agent;
    uint32_t* counter = (uint32_t*)data;
    (*counter)++;
    return HSA_STATUS_SUCCESS;
}


int lsgpu_query_init()
{
    CHECK_HSA(hsa_init()); // Initialize HSA runtime
    return 0;
}

int lsgpu_query_fini()
{
    CHECK_HSA(hsa_shut_down()); // Shut down HSA runtime
    return 0;
}


int lsgpu_query_gpus_data(lsgpu_gpu_list_t* gpu_list) 
{
    uint32_t gpu_count = 0;
    CHECK_HSA(hsa_iterate_agents(count_agents, &gpu_count));

    gpu_list->count = 0;
    gpu_list->entries = (lsgpu_gpu_data_t*) calloc(gpu_count, sizeof(lsgpu_gpu_data_t));
    if (!gpu_list->entries) {
        return -1;
    }

    CHECK_HSA(hsa_iterate_agents(query_agent_data, gpu_list));

    return 0;
}