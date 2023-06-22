#include "symboltable.h"
#include "abstract-syntax-tree.h"
#include <stdlib.h>
#include <string.h>
#include "../support/khash.h"


KHASH_MAP_INIT_STR(variables_hash_map, sm_variable *)

static khash_t(variables_hash_map) * variables_hm;

void init_symbol_table(void) {
    variables_hm = kh_init(variables_hash_map);
}

static void free_nodes(NodeList *nodes) {
    if (nodes == NULL) return;
    free_nodes(nodes->next);
    free(nodes->name);
    free(nodes);
}

static void free_variable(sm_variable *var) {
    free_nodes(var->nodesA);
    free(var);
}

void free_symbol_table(void) {
    sm_variable *var;
    kh_foreach_value(variables_hm, var, free_variable(var));
    kh_destroy(variables_hash_map, variables_hm);
}

int symbol_table_exists(char *name) {
    khiter_t k = kh_get(variables_hash_map, variables_hm, name);
    return k != kh_end(variables_hm);
}

int symbol_table_putGraph(char *name) {
    if (!symbol_table_exists(name)) {
        int ret;
        khiter_t k = kh_put(variables_hash_map, variables_hm, name, &ret);
        if (ret == -1) {
            return -1;
        }

        sm_variable *var = malloc(sizeof(sm_variable));
        var->nodesA = NULL;
        kh_val(variables_hm, k) = var;
        return 0;
    }

    return -1;
}

NodeList *createNode(char *name) {
    NodeList *ret = malloc(sizeof(NodeList));
    size_t length = strlen(name);
    ret->name = malloc(length + 1);
    strcpy(ret->name, name);
    return ret;
}

NodeList *addNodeRecursive(NodeList *nodes, char *name, int *repeated) {
    int cmp;
    if (nodes == NULL || (cmp = strcmp(nodes->name, name)) > 0) {
        NodeList *aux = createNode(name);
        aux->next = (nodes == NULL) ? NULL : nodes->next;
        return aux;
    }
    if (cmp == 0) {
        *repeated = 1;
        return nodes->next;
    }
    nodes->next = addNodeRecursive(nodes->next, name, repeated);
    return nodes;
}

int symbol_table_addNode(char *graphName, char *name) {
    khiter_t k = kh_get(variables_hash_map, variables_hm, graphName);
    if (k != kh_end(variables_hm)) {
        sm_variable *var = kh_val(variables_hm, k);
        NodeList *aux = var->nodesA;
        if (var->nodesA != NULL) {
            int repeated = 0;
            var->nodesA = addNodeRecursive(var->nodesA, name, &repeated);
            if (repeated) {
                return -1;
            }
        } else {
            var->nodesA = createNode(name);
        }
        return 0;
    }
    return -1;
}

NodeList *removeNode(NodeList *nodes, char *name) {
    if (nodes == NULL) {
        return NULL;
    }

    if (strcmp(nodes->name, name) == 0) {
        NodeList *aux = nodes->next;
        free(nodes);
        return aux;
    }

    nodes->next = removeNode(nodes->next, name);
    return nodes;
}

int symbol_table_removeNodes(char *graphName, NodeList *nodes) {
    khiter_t k = kh_get(variables_hash_map, variables_hm, graphName);
    if (k == kh_end(variables_hm)) {
        return -1;
    }
    sm_variable *saved = kh_val(variables_hm, k);
    NodeList *savedNodes = saved->nodesA;
    while (nodes != NULL) {
        savedNodes = removeNode(savedNodes, nodes->name);
        nodes = nodes->next;
    }
    saved->nodesA = savedNodes;
    return 0;
}

