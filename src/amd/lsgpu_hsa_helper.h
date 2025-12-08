#ifndef LSGPU_HSA_HELPER_H
#define LSGPU_HSA_HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <hsa/hsa.h>
#include <hsa/hsa_ext_amd.h>


#define QUERY(INFO, var) hsa_agent_get_info(agent, (hsa_agent_info_t) INFO, var)


static inline uint32_t get_node_id(hsa_agent_t agent) {
    uint32_t node;
    hsa_agent_get_info(agent, HSA_AGENT_INFO_NODE, &node);
    return node;
}

static inline uint32_t get_nearest_cpu_node(hsa_agent_t agent) {
    hsa_agent_t cpu;
    hsa_agent_get_info(agent, HSA_AMD_AGENT_INFO_NEAREST_CPU, &cpu);
    return get_node_id(cpu);
}

static inline char* get_isa_name(hsa_agent_t agent) {
    hsa_isa_t isa;
    hsa_agent_get_info(agent, HSA_AGENT_INFO_ISA, &isa);
    char* name = NULL;
    uint32_t len = 0;
    hsa_isa_get_info_alt(isa, HSA_ISA_INFO_NAME_LENGTH, &len);
    if (len != 0) {
        name = (char*) calloc(len, 1);
        hsa_isa_get_info_alt(isa, HSA_ISA_INFO_NAME, name);
    }
    return name;
}

static inline bool agent_is_gpu(hsa_agent_t agent) {
    uint32_t type = 0;
    hsa_agent_get_info(agent, HSA_AGENT_INFO_DEVICE, &type);
    return type == 1;
}

static inline const char* hsa_err_string(hsa_status_t status)
{
    const char* msg = NULL;
    if (hsa_status_string(status, &msg) != HSA_STATUS_SUCCESS)
        return "<unknown HSA status>";
    return msg ? msg : "<null>";
}

#define CHECK_HSA(call) do { \
    hsa_status_t _s = (call); \
    if (_s != HSA_STATUS_SUCCESS) { \
        fprintf(stderr, \
            "[lsgpu] %s:%d: %s failed: %s (%d)\n", \
            __FILE__, __LINE__, #call, hsa_err_string(_s), _s); \
        return -1; \
    } \
} while (0)



#endif // LSGPU_HSA_HELPER_H