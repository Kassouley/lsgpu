#ifndef LSGPU_H
#define LSGPU_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    LSGPU_TYPE_OFFSET = 0,
    LSGPU_NODE_ID_OFFSET = 4
} lsgpu_attribute_offsets_t;

typedef enum {
    LSGPU_TYPE_WIDTH = 4,
    LSGPU_NODE_ID_WIDTH = 8
} lsgpu_attribute_width_t;

typedef enum {
    LSGPU_ATTRIBUTE_TYPE,
    LSGPU_ATTRIBUTE_NODE_ID
} lsgpu_attribute_t;


static inline void print_label(const char* label, const char* prefix) {
    const int column_width = 28;

    // Compute prefix length after the last newline
    const char* last_nl = strrchr(prefix, '\n');
    const char* visible_prefix = last_nl ? last_nl + 1 : prefix;
    int prefix_len = (int)strlen(visible_prefix);

    int label_len = (int)strlen(label);

    int spaces = column_width - prefix_len - label_len - 1;
    if (spaces < 1) spaces = 1;

    printf("%s%s:", prefix, label);

    // Print spacing
    for (int i = 0; i < spaces; i++)
        printf(" ");
}

typedef struct lsgpu_gpu_data_s* lsgpu_gpu_data_t;

typedef struct {
    uint32_t count;
    lsgpu_gpu_data_t* entries;
} lsgpu_gpu_list_t;

int lsgpu_query_init();
int lsgpu_query_fini();
int lsgpu_query_gpus_data(lsgpu_gpu_list_t* gpu_list);

void lsgpu_print_gpu_data(lsgpu_gpu_data_t gpu);
void lsgpu_print_gpus_data(lsgpu_gpu_list_t* gpu_list);

void lsgpu_to_json_gpu_data(const char* filename, lsgpu_gpu_data_t gpu);
void lsgpu_to_json_gpus_data(const char* prefix_filename, lsgpu_gpu_list_t *gpu_list);

void lsgpu_get_attribute(lsgpu_gpu_data_t gpu, lsgpu_attribute_t attribute, void* out);

void lsgpu_create_gpu_data(lsgpu_gpu_data_t* gpu);
void lsgpu_destroy_gpu_data(lsgpu_gpu_data_t gpu);

int __lsgpu_read_gpu_data_binary_impl(lsgpu_gpu_list_t *gpu_list, uint8_t* buf, size_t size);
int __lsgpu_write_gpu_data_binary_impl(const lsgpu_gpu_list_t *gpu_list, FILE* fp);

int lsgpu_write_gpu_data_binary(const lsgpu_gpu_list_t *gpu_list, const char *filename);
int lsgpu_read_gpu_data_binary(lsgpu_gpu_list_t *gpu_list, const char *filename);
int lsgpu_read_gpu_data_from_buffer(lsgpu_gpu_list_t *gpu_list, uint8_t *buffer, size_t size);

#endif // LSGPU_H