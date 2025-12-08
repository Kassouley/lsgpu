#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "lsgpu.h"

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

    /* Write number of GPU entries */
    if (fwrite(&gpu_list->count, sizeof(gpu_list->count), 1, fp) != 1) {
        perror("fwrite count");
        fclose(fp);
        return -1;
    }

    int status = __lsgpu_write_gpu_data_binary_impl(gpu_list, fp);

    fclose(fp);
    return status;
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
    if (fread(&gpu_list->count, sizeof(gpu_list->count), 1, fp) != 1) {
        perror("fread count");
        fclose(fp);
        return -1;
    }

    /* Allocate memory for GPU entries */
    gpu_list->entries = (lsgpu_gpu_data_t*)calloc(gpu_list->count, sizeof(lsgpu_gpu_data_t));
    if (!gpu_list->entries) {
        perror("calloc");
        fclose(fp);
        return -1;
    }

    int status = __lsgpu_read_gpu_data_binary_impl(gpu_list, fp);

    fclose(fp);
    return status;
}