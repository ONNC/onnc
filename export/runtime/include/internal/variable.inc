#ifdef MKLDNN
#include "mkldnn.h"
#include <stdio.h>
#define CHECK(f)                                                             \
    do {                                                                     \
        mkldnn_status_t s = f;                                               \
        if (s != mkldnn_success) {                                           \
            printf("[%s:%d] error: %s returns %d\n", __FILE__, __LINE__, #f, \
                    s);                                                      \
            exit(2);                                                         \
        }                                                                    \
    } while (0)
typedef struct {
        int nargs;
            mkldnn_exec_arg_t *args;
} args_t;
static void prepare_arg_node(args_t *node, int nargs) {
        node->args = (mkldnn_exec_arg_t *)malloc(sizeof(mkldnn_exec_arg_t) * nargs);
            node->nargs = nargs;
}
static void set_arg(
                mkldnn_exec_arg_t *arg, int arg_idx, mkldnn_memory_t memory) {
        arg->arg = arg_idx;
            arg->memory = memory;
}
mkldnn_engine_t engine;
mkldnn_stream_t stream;
#endif
