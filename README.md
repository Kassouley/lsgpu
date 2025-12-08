# LSGPU - GPU Device Information Utility

LSGPU is a utility for querying GPU device information. It provides a C library and command-line interface to detect, enumerate, and serialize GPU properties across different hardware vendors.

## Features

- **GPU Device Enumeration**: Query available GPU devices on the system
- **Device Information**: Retrieve hardware specifications
- **Binary Serialization**: Encode/decode GPU information to/from binary format for efficient storage and transmission
- **Lua Integration**: Lua bindings for use in profiling and analysis tools
- **Vendor Support**: Modular design supporting multiple GPU vendors

## Supported Vendors

### AMD
AMD GPU support is fully implemented using the HSA (Heterogeneous System Architecture) API. The library queries AMD GPUs through the HSA runtime environment.

### NVIDIA (Not Yet Implemented)
NVIDIA GPU support is planned but not yet implemented. The project structure includes placeholders for NVIDIA support, but the actual implementation is pending.

## Building

LSGPU requires:
- CMake 3.10+
- A C compiler supporting C99 or later
- Vendor-specific headers and libraries (AMD HSA for AMD support)
- Lua 5.1 (optional, only for Lua build)


### Build Steps

- Build lsgpu CLI :
```bash
cd lsgpu
mkdir build
cd build

# For AMD GPU support
cmake -DLSGPU_VENDOR=AMD \
       -DLSGPU_VENDOR_INCLUDE_DIR=/path/to/rocm/include \
       ..

# For NVIDIA GPU support (not yet implemented)
# cmake -DLSGPU_VENDOR=NVIDIA \
#        -DLSGPU_VENDOR_INCLUDE_DIR=/path/to/cuda/include \
#        ..

make
```
- Build lsgpu Lua :
```bash
cmake -DLSGPU_VENDOR=<VENDOR> \
       -DLSGPU_VENDOR_INCLUDE_DIR=/path/to/vendor/include \
       -DLSGPU_BUILD_LUA=ON -DLSGPU_BUILD_CLI=OFF \
       -DLUA_INCLUDE_DIR=/path/to/lua5.1/include \
       ..

make
```

## Usage

### Command-Line Interface

```bash
# List all GPU devices
./lsgpu

# Encode GPU information to binary file
./lsgpu -b output.gpu

# Decode GPU information from binary file
./lsgpu -d output.gpu
```

### C Library

```c
#include "lsgpu.h"

int main() {
    lsgpu_gpu_list_t devices = {0};
    
    if (lsgpu_init() != 0) {
        return 1;
    }
    
    if (lsgpu_query_gpus_data(&devices) != 0) {
        return 1;
    }
    
    lsgpu_print_gpus_data(&devices);
    
    lsgpu_fini();
    return 0;
}
```

### Lua Integration

The Lua wrapper (`lsgpu_wrapper_lua.c`) exposes GPU device information to Lua scripts through the `lsgpu` interface.

## Architecture

- `src/lsgpu.h` - Main public API
- `src/lsgpu_cli.c` - CLI Source
- `src/lsgpu.c` - Common functionality
- `src/amd/` - AMD/HSA implementation
- `src/nvidia/` - NVIDIA implementation (placeholder)
- `lua/` - Lua language bindings

