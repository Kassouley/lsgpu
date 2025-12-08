#include <stdio.h>
#include "lsgpu.h"

int main(int argc, char *argv[]) {
    lsgpu_gpu_list_t devices = {0};
    const char *binary_filename = NULL;

    int decode = 0;
    int encode = 0;

    /* Parse command-line arguments for "-b <filename>" */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-b") == 0) {
            encode = 1;
            if (i + 1 < argc) {
                binary_filename = argv[i + 1];
                i++; /* skip filename */
            } else {
                fprintf(stderr, "Error: -b requires a filename\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-d") == 0) {
            decode = 1;
            if (i + 1 < argc) {
                binary_filename = argv[i + 1];
                i++; /* skip filename */
            } else {
                fprintf(stderr, "Error: -d requires a filename\n");
                return 1;
            }
        }
    }
    
    if(lsgpu_init() != 0) {
        fprintf(stderr, "Failed to init lsgpu\n");
        return 1;
    }

    if (lsgpu_query_gpus_data(&devices) != 0) {
        fprintf(stderr, "Failed to query GPU devices\n");
        return 1;
    }
    
    if (encode) {
        if (lsgpu_write_gpu_data_binary(&devices, binary_filename) != 0) {
            fprintf(stderr, "Failed to write GPU data to binary file '%s'\n", binary_filename);
        } else {
            printf("GPU data written to binary file '%s' successfully.\n", binary_filename);
        }
    } else if (encode) {
        if (lsgpu_read_gpu_data_binary(&devices, binary_filename) != 0) {
            fprintf(stderr, "Failed to read GPU data from binary file '%s'\n", binary_filename);
        } else {
            lsgpu_print_gpus_data(&devices);
        }
    } else {
        lsgpu_print_gpus_data(&devices);
    }


    free(devices.entries);
    
    if(lsgpu_fini() != 0) {
        fprintf(stderr, "Failed to fini lsgpu\n");
        return 1;
    }

    return 0;
}