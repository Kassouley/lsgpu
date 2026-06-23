#ifndef LSGPU_AMD_TYPE_H
#define LSGPU_AMD_TYPE_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "lsgpu_type.h"

static inline const char* get_device_type(uint32_t id) {
    static const char* device_types[] = { "CPU", "GPU", "DSP", "AIE" };
    uint32_t count = sizeof(device_types) / sizeof(device_types[0]);
    return (id < count) ? device_types[id] : "-";
}

static inline const char* get_feature_type(uint32_t id) {
    static const char* feature_types[] = { "AGENT DISPATCH", "KERNEL DISPATCH" };
    uint32_t count = sizeof(feature_types) / sizeof(feature_types[0]);
    return (id < count) ? feature_types[id] : "-";
}

static inline const char* get_machine_model_type(uint32_t id) {
    static const char* machine_model_types[] = { "SMALL", "LARGE" };
    uint32_t count = sizeof(machine_model_types) / sizeof(machine_model_types[0]);
    return (id < count) ? machine_model_types[id] : "-";
}

static inline const char* get_queue_type(uint32_t id) {
    static const char* queue_types[] = { "MULTI", "SINGLE", "COOPERATIVE" };
    uint32_t count = sizeof(queue_types) / sizeof(queue_types[0]);
    return (id < count) ? queue_types[id] : "-";
}

typedef u32 queue_type_t;
LSGPU_HELPER_API void print_queue_type_t(FILE* fp, u32 v) { fprintf(fp, "%s", get_queue_type(v)); }
LSGPU_HELPER_API void tojson_queue_type_t(FILE* fp, u32 v) { fprintf(fp, "\"%s\"", get_queue_type(v)); }
LSGPU_HELPER_API int  write_queue_type_t(FILE *fp, u32* v) { return write_u32(fp, v); }
LSGPU_HELPER_API void read_queue_type_t(uint8_t **buf, u32* v)  { read_u32(buf, v); }


typedef u32 device_type_t;
LSGPU_HELPER_API void print_device_type_t(FILE* fp, u32 v) { fprintf(fp, "%s", get_device_type(v)); }
LSGPU_HELPER_API void tojson_device_type_t(FILE* fp, u32 v) { fprintf(fp, "\"%s\"", get_device_type(v)); }
LSGPU_HELPER_API int  write_device_type_t(FILE *fp, u32* v) { return write_u32(fp, v); }
LSGPU_HELPER_API void read_device_type_t(uint8_t **buf, u32* v)  { read_u32(buf, v); }


typedef u32 feature_t;
LSGPU_HELPER_API void print_feature_t(FILE* fp, u32 v) { fprintf(fp, "%s", get_feature_type(v)); }
LSGPU_HELPER_API void tojson_feature_t(FILE* fp, u32 v) { fprintf(fp, "\"%s\"", get_feature_type(v)); }
LSGPU_HELPER_API int  write_feature_t(FILE *fp, u32* v) { return write_u32(fp, v); }
LSGPU_HELPER_API void read_feature_t(uint8_t **buf, u32* v)  { read_u32(buf, v); }


typedef u32 machine_model_t;
LSGPU_HELPER_API void print_machine_model_t(FILE* fp, u32 v) { fprintf(fp, "%s", get_machine_model_type(v)); }
LSGPU_HELPER_API void tojson_machine_model_t(FILE* fp, u32 v) { fprintf(fp, "\"%s\"", get_machine_model_type(v)); }
LSGPU_HELPER_API int  write_machine_model_t(FILE *fp, u32* v) { return write_u32(fp, v); }
LSGPU_HELPER_API void read_machine_model_t(uint8_t **buf, u32* v)  { read_u32(buf, v); }


#endif // LSGPU_AMD_TYPE_H