
#include "../support/khash.h"
#include "symboltable.h"
#include "abstract-syntax-tree.h"
#include <stdlib.h>

KHASH_MAP_INIT_STR(variables_hash_map, GraphList *)

static khash_t(variables_hash_map) * variables_hm;

void init_symbol_table(void) {
    variables_hm = kh_init(variables_hash_map);
}

void free_symbol_table(void) {
    kh_destroy(variables_hash_map, variables_hm);
}

int symbol_table_exists(char *name) {
    khiter_t k = kh_get(variables_hash_map, variables_hm, name);
    return k != kh_end(variables_hm);
}

int symbol_table_putGraph(char *name, GraphList *graph) {
    if (!symbol_table_exists(name)) {
        int ret;
        khiter_t k = kh_put(variables_hash_map, variables_hm, name, &ret);
        if (ret == -1) {
            return -1;
        }

        kh_val(variables_hm, k) = graph;
        return 0;
    }

    return -1;
}
