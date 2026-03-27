#ifndef LSGPU_AMD_TYPE_H
#define LSGPU_AMD_TYPE_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <hsa/hsa.h>
#include <hsa/hsa_ext_amd.h>
#include "lsgpu_type.h"
#include "lsgpu_amd.h"
#include "lsgpu_hsa_helper.h"

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