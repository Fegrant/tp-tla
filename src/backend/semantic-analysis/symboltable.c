#include "symboltable.h"
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

static void free_edges(EdgeList *edges) {
    if (edges == NULL) return;
    free_edges(edges->next);
    free(edges->leftNode);
    free(edges->rightNode);
    free(edges);
}

static void free_variable(sm_variable *var) {
    free_nodes(var->nodesA);
    free_edges(var->edgesA);
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
            return -1;         // ???
        }

        sm_variable *var = calloc(1, sizeof(sm_variable));
        kh_val(variables_hm, k) = var;
        return NO_ERRORS;
    }

    return GRAPH_ALREADY_EXISTS;
}

int symbol_table_nodeExists(char * graphName, char * name) {
    khiter_t k = kh_get(variables_hash_map, variables_hm, graphName);
    if (k != kh_end(variables_hm)) {
        sm_variable *var = kh_val(variables_hm, k);
        NodeList *aux = var->nodesA;
        if (!nodeExistsRecursive(aux, name)) {
            return NODE_NOT_EXISTS;
        }
        return NO_ERRORS;
    }
    return GRAPH_NOT_EXISTS;
}

int symbol_table_addNode(char *graphName, char *name) {
    khiter_t k = kh_get(variables_hash_map, variables_hm, graphName);
    if (k != kh_end(variables_hm)) {
        sm_variable *var = kh_val(variables_hm, k);
        NodeList *aux = var->nodesA;
        if (var->nodesA != NULL) {
            int repeated = FALSE;
            var->nodesA = addNodeRecursive(var->nodesA, name, &repeated);
            if (repeated == TRUE) {
                return NODE_ALREADY_EXISTS;
            }
        } else {
            var->nodesA = createNode(name);
        }
        return NO_ERRORS;
    }
    return GRAPH_NOT_EXISTS;
}

int symbol_table_addEdge(char* graphName, char* leftNodeName, char* rightNodeName, int weight) {
    khiter_t k = kh_get(variables_hash_map, variables_hm, graphName);
    if (k != kh_end(variables_hm)) {
        sm_variable *saved = kh_val(variables_hm, k);
        if (!nodeExistsRecursive(saved->nodesA, leftNodeName) || !nodeExistsRecursive(saved->nodesA, rightNodeName)) {
            return NODE_NOT_EXISTS;              // One of edges' nodes don´t exist
        }
        EdgeList *savedEdges = saved->edgesA;
        char *firstNodeName;
        char *secondNodeName;
        if (strcmp(leftNodeName, rightNodeName) <= 0) {
            firstNodeName = leftNodeName;
            secondNodeName = rightNodeName;
        } else {
            firstNodeName = rightNodeName;
            secondNodeName = leftNodeName;
        }
        if (saved->edgesA != NULL) {
            int repeated = FALSE;
            saved->edgesA = addEdgeRecursive(saved->edgesA, firstNodeName, secondNodeName, weight, &repeated);
            if (repeated == TRUE) {
                return EDGE_ALREADY_EXISTS;              // Edge already exists
            }
        } else {
            saved->edgesA = createEdge(firstNodeName, secondNodeName, weight);
        }
        return NO_ERRORS;
    }
    return GRAPH_NOT_EXISTS;                          // Graph doesn´t exist
}

int symbol_table_removeEdge(char *graphName, char *leftNodeName, char *rightNodeName, int weight) {
    khiter_t k = kh_get(variables_hash_map, variables_hm, graphName);
    if (k == kh_end(variables_hm)) {
        return GRAPH_NOT_EXISTS;                      //  Graph doesn´t exist
    }
    sm_variable *saved = kh_val(variables_hm, k);
    if (!nodeExistsRecursive(saved->nodesA, leftNodeName) || !nodeExistsRecursive(saved->nodesA, rightNodeName)) {
        return NODE_NOT_EXISTS;              // One of edges' nodes don´t exist
    }
    EdgeList *savedEdges = saved->edgesA;
    char *firstNodeName;
    char *secondNodeName;
    if (strcmp(leftNodeName, rightNodeName) <= 0) {
        firstNodeName = leftNodeName;
        secondNodeName = rightNodeName;
    } else {
        firstNodeName = rightNodeName;
        secondNodeName = leftNodeName;
    }
    if (savedEdges != NULL) {
        int wasFound = FALSE;
        savedEdges = removeEdgeRecursive(savedEdges, firstNodeName, secondNodeName, weight, &wasFound);
        if (wasFound == FALSE) {        // Edge not exists
            return EDGE_NOT_EXISTS;
        }
    } else {
        return EDGE_NOT_EXISTS;              // Graph didn´t have edges -> edge didn´t exist
    }
    saved->edgesA = savedEdges;
    return NO_ERRORS;
}

int symbol_table_removeNode(char *graphName, char *name) {
    khiter_t k = kh_get(variables_hash_map, variables_hm, graphName);
    if (k == kh_end(variables_hm)) {
        return GRAPH_NOT_EXISTS;
    }
    sm_variable *saved = kh_val(variables_hm, k);
    NodeList *savedNodes = saved->nodesA;
    if (savedNodes != NULL) {
        int wasFound = FALSE;
        savedNodes = removeNodeRecursive(savedNodes, name, &wasFound);
        if (wasFound == FALSE) {        // Node didn´t exist
            return NODE_NOT_EXISTS;
        }
    } else {
        return NODE_NOT_EXISTS;              // Graph didn´t have nodes -> node didn´t exist
    }
    saved->nodesA = savedNodes;
    return NO_ERRORS;
}

/*
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
*/

