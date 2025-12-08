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
    agent_data->isa = get_isa_name(agent);

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


void lsgpu_print_gpus_data(lsgpu_gpu_list_t *gpu_list) 
{
    for (size_t i = 0; i < gpu_list->count; i++)
    {
        printf("**GPU Device #%lu\n", i+1);
        lsgpu_print_gpu_data(&gpu_list->entries[i]);
    }
}



int lsgpu_write_gpu_data_binary(const lsgpu_gpu_list_t *gpu_list, const char *filename)
{
    if (!gpu_list || !filename) return -1;

    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("fopen");
        return -1;
    }

    /* Write number of GPU entries first */
    if (fwrite(&gpu_list->count, sizeof(gpu_list->count), 1, fp) != 1) {
        perror("fwrite count");
        fclose(fp);
        return -1;
    }

    for (size_t i = 0; i < gpu_list->count; i++)
    {
        #define WRITE_FIELD(prefix, label, type, name, _) \
            write_##type(fp, gpu_list->entries[i].name);
        FOR_EACH_FIELD(WRITE_FIELD)
        #undef WRITE_FIELD
    }

    fclose(fp);
    return 0;
}


int lsgpu_read_gpu_data_binary(lsgpu_gpu_list_t *gpu_list, const char *filename)
{
    if (!gpu_list || !filename) return -1;

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("fopen");
        return -1;
    }

    /* Read number of GPU entries */
    size_t count = 0;
    if (fread(&count, sizeof(count), 1, fp) != 1) {
        perror("fread count");
        fclose(fp);
        return -1;
    }

    /* Allocate memory for GPU entries */
    gpu_list->entries = (lsgpu_gpu_data_t*)calloc(count, sizeof(lsgpu_gpu_data_t));
    if (!gpu_list->entries) {
        fclose(fp);
        return -1;
    }
    gpu_list->count = count;

    /* Read each GPU entry field-by-field */
    for (size_t i = 0; i < count; i++) {
        #define READ_FIELD(prefix, label, type, name, _) \
            if (read_##type(fp, &gpu_list->entries[i].name) != 0) { \
                fclose(fp); \
                free(gpu_list->entries); \
                return -1;\
            }

        FOR_EACH_FIELD(READ_FIELD)
        #undef READ_FIELD
    }

    fclose(fp);
    return 0;
}