#ifndef LSGPU_TYPE_H
#define LSGPU_TYPE_H

#include <stdio.h>
#include <stdint.h>

#define LSGPU_HELPER_API static inline


#define write_to_file(fp, v)  fwrite(v, sizeof(*(v)), 1, fp);
#define read_from_file(fp, v) fread(v, sizeof(*(v)), 1, fp);


/* Strings: write until null terminator */
static inline int write_str_to_file(FILE *fp, char *v, size_t max_len) {
    for (size_t i = 0; i < max_len; i++) {
        if (fputc(v[i], fp) == EOF) return -1;
        if (v[i] == '\0') break;
    }
    return 1;
}

/* Strings: read until null terminator */
static inline int read_str_from_file(FILE *fp, char *v, size_t max_len) {
    if (!fp || !v) return -1;
    size_t i = 0;
    int c;
    while (i < max_len) {
        c = fgetc(fp);
        if (c == EOF) return -1;
        v[i++] = (char)c;
        if (c == '\0') break;
    }
    if (i == max_len) v[max_len - 1] = '\0';
    return 1;
}


/** String 64-bytes type helper */
typedef char str64[64];

LSGPU_HELPER_API void print_str64(char v[64]) { printf("%s\n", v); }
LSGPU_HELPER_API int  write_str64(FILE *fp, char (*v)[64]) { return write_str_to_file(fp, *v, 64); }
LSGPU_HELPER_API int  read_str64(FILE *fp, char (*v)[64])  { return read_str_from_file(fp, *v, 64); }


/** String 21-bytes type helper */
typedef char str21[21];

LSGPU_HELPER_API void print_str21(char v[21]) { printf("%s\n", v); }
LSGPU_HELPER_API int  write_str21(FILE *fp, char (*v)[21]) { return write_str_to_file(fp, *v, 21); }
LSGPU_HELPER_API int  read_str21(FILE *fp, char (*v)[21])  { return read_str_from_file(fp, *v, 21); }


/** Unsigned Int 32 type helper */
typedef uint32_t u32;

LSGPU_HELPER_API void print_u32(u32 v) { printf("%u\n", v); }
LSGPU_HELPER_API int  write_u32(FILE *fp, u32 *v) { return write_to_file(fp, v); }
LSGPU_HELPER_API int  read_u32(FILE *fp, u32 *v)  { return read_from_file(fp, v); }


/** Unsigned Int 16 type helper */
typedef uint16_t u16;

LSGPU_HELPER_API void print_u16(u16 v) { printf("%u\n", v); }
LSGPU_HELPER_API int  write_u16(FILE *fp, u16 *v) { return write_to_file(fp, v); }
LSGPU_HELPER_API int  read_u16(FILE *fp, u16 *v)  { return read_from_file(fp, v); }


/** Unsigned Int 16 Array[3] type helper */
typedef uint16_t u16x3[3];

LSGPU_HELPER_API void print_u16x3(u16x3 v) { printf("[%u, %u, %u]\n", v[0], v[1], v[2]); }
LSGPU_HELPER_API int  write_u16x3(FILE *fp, u16x3 *v) { return write_to_file(fp, v); }
LSGPU_HELPER_API int  read_u16x3(FILE *fp, u16x3 *v)  { return read_from_file(fp, v); }


/** Unsigned Int 32 Array[3] type helper */
typedef uint32_t u32x3[3];

LSGPU_HELPER_API void print_u32x3(u32x3 v) { printf("[%u, %u, %u]\n", v[0], v[1], v[2]); }
LSGPU_HELPER_API int  write_u32x3(FILE *fp, u32x3 *v) { return write_to_file(fp, v); }
LSGPU_HELPER_API int  read_u32x3(FILE *fp, u32x3 *v)  { return read_from_file(fp, v); }


/** Unsigned Int 32 Array[4] type helper */
typedef uint32_t u32x4[4];

LSGPU_HELPER_API void print_u32x4(u32x4 v) { printf("[%u, %u, %u, %u]\n", v[0], v[1], v[2], v[3]); }
LSGPU_HELPER_API int  write_u32x4(FILE *fp, u32x4 *v) { return write_to_file(fp, v); }
LSGPU_HELPER_API int  read_u32x4(FILE *fp, u32x4 *v)  { return read_from_file(fp, v); }



/** Unsigned Int 32 Array[4] alias Cache type helper */
typedef u32x4 cache_t;

LSGPU_HELPER_API void print_cache_t(cache_t v) {
    const char* cache_labels[] = { "L1", "L2", "L3", "L4" };
    printf("\n");
    for (size_t j = 0; j < 4; j++)
    {
        if (v[j] != 0) {
            printf("\t%s: %-10u (0x%x) KB\n", cache_labels[j], v[j], v[j]);
        }
    }
}

LSGPU_HELPER_API int write_cache_t(FILE *fp, cache_t *v) { return write_u32x4(fp, v); }
LSGPU_HELPER_API int read_cache_t(FILE *fp, cache_t* v)  { return read_u32x4(fp, v); }


/** Unsigned Int 32 Array[3] alias XYZ type helper */
typedef u32x3 u32_xyz_t;

LSGPU_HELPER_API void print_u32_xyz_t(u32_xyz_t v) {
    const char* xyz_labels[]   = { "x", "y", "z" };
    printf("\n");
    for (size_t j = 0; j < 3; j++)
        printf("\t%s: %u\n", xyz_labels[j], v[j]);
}

LSGPU_HELPER_API int write_u32_xyz_t(FILE *fp, u32_xyz_t *v) { return write_u32x3(fp, v); }
LSGPU_HELPER_API int read_u32_xyz_t(FILE *fp, u32_xyz_t* v)  { return read_u32x3(fp, v); }


/** Unsigned Int 16 Array[3] alias XYZ type helper */
typedef u16x3 u16_xyz_t;

LSGPU_HELPER_API void print_u16_xyz_t(u16_xyz_t v) {
    u32_xyz_t tmp = { v[0], v[1], v[2] };
    print_u32_xyz_t(tmp);
}

LSGPU_HELPER_API int write_u16_xyz_t(FILE *fp, u16_xyz_t *v) { return write_u16x3(fp, v); }
LSGPU_HELPER_API int read_u16_xyz_t(FILE *fp, u16_xyz_t* v)  { return read_u16x3(fp, v); }

#endif // LSGPU_TYPE_H
