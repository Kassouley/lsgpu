#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <hsa/hsa.h>
#include <hsa/hsa_ext_amd.h>

#include "lsgpu.h"
#include "lsgpu_type.h"
#include "lsgpu_amd_type.h"
#include "lsgpu_amd.h"
#include "lsgpu_hsa_helper.h"


static hsa_status_t query_agent_data(hsa_agent_t agent, void* data) 
{
    if (!agent_is_gpu(agent)) return HSA_STATUS_SUCCESS;

    lsgpu_gpu_list_t *agents_list = (lsgpu_gpu_list_t*)data;
    lsgpu_gpu_data_t* agent_data = &agents_list->entries[agents_list->count++];

    #define QUERY_FIELD(___, _, __, field, info) \
        if (info != -1) hsa_agent_get_info(agent, (hsa_agent_info_t) info, &agent_data->field);
    FOR_EACH_FIELD(QUERY_FIELD)
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


int lsgpu_init()
{
    CHECK_HSA(hsa_init()); // Initialize HSA runtime
    return 0;
}

int lsgpu_fini()
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

void lsgpu_print_gpu_data(lsgpu_gpu_data_t *gpu)
{
    #define PRINT_FIELD(prefix, label, type, name, _) \
        print_label(label, prefix); \
        print_##type(gpu->name);
    FOR_EACH_FIELD(PRINT_FIELD)
    #undef PRINT_FIELD
}



int __lsgpu_write_gpu_data_binary_impl(const lsgpu_gpu_list_t *gpu_list, FILE* fp)
{
    for (size_t i = 0; i < gpu_list->count; i++)
    {
        #define WRITE_FIELD(___, __, type, name, _) \
            if (write_##type(fp, &gpu_list->entries[i].name) != 1) { \
                fprintf(stderr, "error: write_"#type"\n"); \
                return -1; \
            }
        FOR_EACH_FIELD(WRITE_FIELD)
        #undef WRITE_FIELD
    }

    return 0;
}


int __lsgpu_read_gpu_data_binary_impl(lsgpu_gpu_list_t *gpu_list, FILE* fp)
{
    for (size_t i = 0; i < gpu_list->count; i++) 
    {
        #define READ_FIELD(___, __, type, name, _) \
            if (read_##type(fp, &gpu_list->entries[i].name) != 1) { \
                fprintf(stderr, "error: read_"#type"\n"); \
                return -1;\
            }
        FOR_EACH_FIELD(READ_FIELD)
        #undef READ_FIELD
    }

    return 0;
}