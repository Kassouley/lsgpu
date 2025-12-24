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



static int load_file_to_buffer(const char *filename, uint8_t **buffer, size_t *size) {
    if (!filename || !buffer || !size) return -1;

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("fopen");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    if (file_size < 0) {
        perror("ftell");
        fclose(fp);
        return -1;
    }
    rewind(fp);

    *buffer = (uint8_t*)malloc(file_size);
    if (!*buffer) {
        perror("malloc");
        fclose(fp);
        return -1;
    }

    if (fread(*buffer, 1, file_size, fp) != (size_t)file_size) {
        perror("fread");
        free(*buffer);
        fclose(fp);
        return -1;
    }

    *size = (size_t)file_size;
    fclose(fp);
    return 0;
}


int lsgpu_read_gpu_data_from_buffer(lsgpu_gpu_list_t *gpu_list, uint8_t *buffer, size_t size) {
    if (!gpu_list || !buffer || size < sizeof(int)) return -1;

    // Read GPU count
    if (sizeof(gpu_list->count) > size) {
        fprintf(stderr, "error: unbound buffer\n");
        return -1;
    }
    memcpy(&gpu_list->count, buffer, sizeof(gpu_list->count));
    buffer += sizeof(gpu_list->count);

    // Allocate GPU entries
    gpu_list->entries = (lsgpu_gpu_data_t*)calloc(gpu_list->count, sizeof(lsgpu_gpu_data_t));
    if (!gpu_list->entries) {
        perror("calloc");
        return -1;
    }

    return __lsgpu_read_gpu_data_binary_impl(gpu_list, buffer, size);
}


int lsgpu_read_gpu_data_binary(lsgpu_gpu_list_t *gpu_list, const char *filename) {
    uint8_t *buffer = NULL;
    size_t size = 0;

    if (load_file_to_buffer(filename, &buffer, &size) != 0) {
        return -1;
    }

    int status = lsgpu_read_gpu_data_from_buffer(gpu_list, buffer, size);
    free(buffer);
    return status;
}