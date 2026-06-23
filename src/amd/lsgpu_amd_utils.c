#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

#include "lsgpu.h"
#include "lsgpu_type.h"
#include "amd/lsgpu_amd_type.h"
#include "amd/lsgpu_amd_utils.h"


void lsgpu_create_gpu_data(lsgpu_gpu_data_t* gpu)
{
    struct lsgpu_gpu_data_s* gpu_data = malloc(sizeof(struct lsgpu_gpu_data_s));
    if (!gpu_data) {
        perror("lsgpu - Error: failed to open file");
        return;
    }
    *gpu = gpu_data;
}

void lsgpu_destroy_gpu_data(lsgpu_gpu_data_t gpu)
{
    struct lsgpu_gpu_data_s* gpu_data = (struct lsgpu_gpu_data_s*) gpu;
    free(gpu_data);
}


void lsgpu_print_gpu_data(lsgpu_gpu_data_t gpu)
{
    struct lsgpu_gpu_data_s* gpu_data = (struct lsgpu_gpu_data_s*) gpu;

    #define PRINT_FIELD(prefix, label, type, name) \
        print_label(label, prefix); \
        print_##type(stdout, gpu_data->name); \
        printf("\n");

    FOR_EACH_FIELD(PRINT_FIELD)
    #undef PRINT_FIELD
}


void lsgpu_to_json_gpu_data(const char* filename, lsgpu_gpu_data_t gpu)
{
    struct lsgpu_gpu_data_s* gpu_data = (struct lsgpu_gpu_data_s*) gpu;

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("lsgpu - Error: failed to open file");
        return;
    }

    int printed_fields = 0;

    fprintf(fp, "{\n");

    #define PRINT_FIELD(prefix, label, type, name) \
        if (printed_fields > 0) fprintf(fp, ",\n"); \
        fprintf(fp, "\t\"%s\": ", label); \
        tojson_##type(fp, gpu_data->name); \
        printed_fields++;

    FOR_EACH_FIELD(PRINT_FIELD)

    #undef PRINT_FIELD

    fprintf(fp, "\n}");
    fclose(fp);
}


int __lsgpu_write_gpu_data_binary_impl(const lsgpu_gpu_list_t *gpu_list, FILE* fp)
{
    for (size_t i = 0; i < gpu_list->count; i++)
    {
        struct lsgpu_gpu_data_s* gpu_data = (struct lsgpu_gpu_data_s*) gpu_list->entries[i];
        #define WRITE_FIELD(_1, _2, type, name) \
            if (write_##type(fp, &gpu_data->name) != 1) { \
                fprintf(stderr, "lsgpu - Error: write_"#type"\n"); \
                return -1; \
            }
        FOR_EACH_FIELD(WRITE_FIELD)
        #undef WRITE_FIELD
    }

    return 0;
}


int __lsgpu_read_gpu_data_binary_impl(lsgpu_gpu_list_t *gpu_list, uint8_t* buf, size_t size)
{
    for (size_t i = 0; i < gpu_list->count; i++) 
    {
        struct lsgpu_gpu_data_s* gpu_data = NULL;
        lsgpu_create_gpu_data(&gpu_data);
        gpu_list->entries[i] = gpu_data;

        #define READ_FIELD(_1, _2, type, name) \
            read_##type(&buf, &gpu_data->name);
        FOR_EACH_FIELD(READ_FIELD)
        #undef READ_FIELD
    }

    return 0;
}