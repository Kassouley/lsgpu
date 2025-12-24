#ifndef LSGPU_H
#define LSGPU_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined(__LSGPU_VENDOR_AMD__)
    #include "amd/lsgpu_amd.h"
#elif defined(__LSGPU_VENDOR_NVIDIA__)
    #include "nvidia/lsgpu_nvidia.h"
#else
    typedef struct lsgpu_gpu_data_s { uint8_t dummy; } lsgpu_gpu_data_t;
    #error "No GPU vendor defined. Please define __LSGPU_VENDOR_AMD__ or __LSGPU_VENDOR_NVIDIA__."
#endif


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


typedef struct {
    uint32_t count;
    lsgpu_gpu_data_t* entries;
} lsgpu_gpu_list_t;

int lsgpu_init();

int lsgpu_fini();

void lsgpu_print_gpu_data(lsgpu_gpu_data_t* gpu);

void lsgpu_print_gpus_data(lsgpu_gpu_list_t* gpu_list) ;

int lsgpu_query_gpus_data(lsgpu_gpu_list_t* gpu_list);


int __lsgpu_read_gpu_data_binary_impl(lsgpu_gpu_list_t *gpu_list, uint8_t* buf, size_t size);
int __lsgpu_write_gpu_data_binary_impl(const lsgpu_gpu_list_t *gpu_list, FILE* fp);

int lsgpu_write_gpu_data_binary(const lsgpu_gpu_list_t *gpu_list, const char *filename);
int lsgpu_read_gpu_data_binary(lsgpu_gpu_list_t *gpu_list, const char *filename);
int lsgpu_read_gpu_data_from_buffer(lsgpu_gpu_list_t *gpu_list, uint8_t *buffer, size_t size);

#endif // LSGPU_H