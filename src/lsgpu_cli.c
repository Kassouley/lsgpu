#include <stdio.h>
#include <stdlib.h>
#include "lsgpu.h"

void usage() {
    fprintf(stderr, "Usage: lsgpu [-b <filename>] | [-d <filename>]\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) 
{
    lsgpu_gpu_list_t devices = {0};

    const char *binary_filename = NULL;
    int decode = 0;
    int encode = 0;

    if (argc != 1 && argc != 3) {
        usage();
    } else if (argc > 1) {
        if (strcmp(argv[1], "-b") == 0) {
            encode = 1;
        } else if (strcmp(argv[1], "-d") == 0) {
            decode = 1;
        } else {
            usage();
        }
        binary_filename = argv[2];
    }
    
    if(lsgpu_init() != 0) {
        fprintf(stderr, "Failed to init lsgpu\n");
        return 1;
    }

    if (decode == 0) {
        if (lsgpu_query_gpus_data(&devices) != 0) {
            fprintf(stderr, "Failed to query GPU devices\n");
            return 1;
        }
    }

    
    if (encode) {
        if (lsgpu_write_gpu_data_binary(&devices, binary_filename) != 0) {
            fprintf(stderr, "Failed to write GPU data to binary file '%s'\n", binary_filename);
        } else {
            printf("GPU data written to binary file '%s' successfully.\n", binary_filename);
        }
    } else if (decode) {
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