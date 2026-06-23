#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "lsgpu.h"

static const uint8_t lsgpu_attribute_offset[] = {
    [LSGPU_ATTRIBUTE_TYPE]    = LSGPU_TYPE_OFFSET,
    [LSGPU_ATTRIBUTE_NODE_ID] = LSGPU_NODE_ID_OFFSET,
};

static const uint8_t lsgpu_attribute_width[] = {
    [LSGPU_ATTRIBUTE_TYPE]    = LSGPU_TYPE_WIDTH,
    [LSGPU_ATTRIBUTE_NODE_ID] = LSGPU_NODE_ID_WIDTH,
};

void lsgpu_get_attribute(lsgpu_gpu_data_t gpu, lsgpu_attribute_t attribute, void* out)
{
    if (!out) return;

    const uint8_t* bytes = (const uint8_t*)gpu;

    uint32_t offset = lsgpu_attribute_offset[attribute];
    uint32_t width  = lsgpu_attribute_width[attribute];

    uint32_t raw = 0;
    memcpy(&raw, bytes + offset, (width + 7) / 8);

    raw &= ((1u << width) - 1u);

    memcpy(out, &raw, sizeof(raw));
}


void lsgpu_print_gpus_data(lsgpu_gpu_list_t *gpu_list) 
{
    for (size_t i = 0; i < gpu_list->count; i++)
    {
        uint32_t node_id = 0;
        lsgpu_get_attribute(gpu_list->entries[i], LSGPU_ATTRIBUTE_NODE_ID, &node_id);
        printf("**GPU Device #%u\n", node_id);
        lsgpu_print_gpu_data(gpu_list->entries[i]);
    }
}

void lsgpu_to_json_gpus_data(const char* prefix_filename, lsgpu_gpu_list_t *gpu_list)
{
    char filename[512];

    for (size_t i = 0; i < gpu_list->count; i++) {
        uint32_t node_id = 0;
        lsgpu_get_attribute(gpu_list->entries[i], LSGPU_ATTRIBUTE_NODE_ID, &node_id);
        snprintf(filename, sizeof(filename), "%s_gpu_%u.json", prefix_filename, node_id);
        lsgpu_to_json_gpu_data(filename, gpu_list->entries[i]);
    }
}



int lsgpu_write_gpu_data_binary(const lsgpu_gpu_list_t *gpu_list, const char *filename)
{
    if (!gpu_list || !filename) return -1;

    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("lsgpu - Error: fopen");
        return -1;
    }

    /* Write number of GPU entries */
    if (fwrite(&gpu_list->count, sizeof(gpu_list->count), 1, fp) != 1) {
        perror("lsgpu - Error: fwrite count");
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
        perror("lsgpu - Error: fopen");
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
        perror("lsgpu - Error: malloc");
        fclose(fp);
        return -1;
    }

    if (fread(*buffer, 1, file_size, fp) != (size_t)file_size) {
        perror("lsgpu - Error: fread");
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
        fprintf(stderr, "lsgpu - Error: unbound buffer\n");
        return -1;
    }
    memcpy(&gpu_list->count, buffer, sizeof(gpu_list->count));
    buffer += sizeof(gpu_list->count);

    // Allocate GPU entries
    gpu_list->entries = (lsgpu_gpu_data_t*)calloc(gpu_list->count, sizeof(lsgpu_gpu_data_t));
    if (!gpu_list->entries) {
        perror("lsgpu - Error: calloc");
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