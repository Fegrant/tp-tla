#include "symboltable.h"
#include "abstract-syntax-tree.h"
#include <stdlib.h>
#include <string.h>
#include "../support/khash.h"

#define TRUE 1
#define FALSE 0

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
    free(((NodeList*)edges->leftNode)->name);
    free(edges->leftNode);
    free(((NodeList*)edges->rightNode)->name);
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

NodeList *createNode(char *name) {
    NodeList *ret = malloc(sizeof(NodeList));
    size_t length = strlen(name);
    ret->name = malloc(length + 1);
    strcpy(ret->name, name);
    return ret;
}

int nodeExistsRecursive(NodeList * nodes, char *name){
    int cmp;
    if (nodes == NULL || (cmp = strcmp(nodes->name, name)) > 0) {
        return FALSE;
    }
    if (cmp == 0) {
        return TRUE;
    }
    return nodeExistsRecursive(nodes->next, name);
}

NodeList *addNodeRecursive(NodeList *nodes, char *name, int *repeated) {
    int cmp;
    if (nodes == NULL || (cmp = strcmp(nodes->name, name)) > 0) {
        *repeated = FALSE;
        NodeList *aux = createNode(name);
        aux->next = (nodes == NULL) ? NULL : nodes;
        return aux;
    }
    if (cmp == 0) {
        *repeated = TRUE;
        return nodes->next;
    }
    nodes->next = addNodeRecursive(nodes->next, name, repeated);
    return nodes;
}

NodeList *removeNodeRecursive(NodeList *nodes, char *name, int *wasFound) {
    int cmp;
    if (nodes == NULL || (cmp = strcmp(nodes->name, name)) > 0) {
        *wasFound = FALSE;
        return nodes;
    }
    if (cmp == 0) {
        *wasFound = TRUE;
        NodeList* next = nodes->next;
        free(nodes->name);
        free(nodes);
        return next;
    }
    nodes->next = removeNodeRecursive(nodes->next, name, wasFound);
    return nodes;
}

EdgeList *createEdge(char *leftNodeName, char* rightNodeName, int weight) {
    EdgeList * edge = calloc(1, sizeof(EdgeList));
    NodeList * leftNode = calloc(1, sizeof(NodeList));
    NodeList * rightNode = calloc(1, sizeof(NodeList));    

    leftNode->name = calloc(1, sizeof(char) * strlen(leftNodeName));
    rightNode->name = calloc(1, sizeof(char) * strlen(rightNodeName));
    strcpy(leftNode->name, leftNodeName);
    strcpy(rightNode->name, rightNodeName);

    edge->leftNode = leftNode;
    edge->rightNode = rightNode;
    edge->weight = weight;
    return edge;
}

EdgeList *addEdgeRecursive(EdgeList * edges, char * leftNodeName, char * rightNodeName, int weight, int *repeated) {
    if (edges == NULL) {
        *repeated = FALSE;
        EdgeList *aux = createEdge(leftNodeName, rightNodeName, weight);
        aux->next = NULL;
        return aux;
    }
    
    NodeList *leftNode = (NodeList *)edges->leftNode;
    NodeList *rightNode = (NodeList *)edges->rightNode;

    int leftCmp = strcmp(leftNode->name, leftNodeName);
    int rightCmp = strcmp(rightNode->name, rightNodeName);

    if (leftCmp > 0 || (leftCmp == 0 && rightCmp > 0)) {
        *repeated = FALSE;
        EdgeList *aux = createEdge(leftNodeName, rightNodeName, weight);
        aux->next = edges;
        return aux;
    }
    if (leftCmp == 0 && rightCmp == 0) {
        *repeated = TRUE;
        return edges->next;
    }
    edges->next = addEdgeRecursive(edges->next, leftNodeName, rightNodeName, weight, repeated);
    return edges;
}

EdgeList *removeEdgeRecursive(EdgeList * edges, char * leftNodeName, char * rightNodeName, int weight, int *wasFound) {
    NodeList *leftNode = (NodeList*)edges->leftNode;
    NodeList *rightNode = (NodeList*)edges->rightNode;

    int leftCmp = strcmp(leftNode->name, leftNodeName);
    int rightCmp = strcmp(rightNode->name, rightNodeName);

    if (edges == NULL || leftCmp > 0 || (leftCmp == 0 && rightCmp > 0)) {
        *wasFound = FALSE;
        return edges;
    }
    if (leftCmp == 0 && rightCmp == 0) {
        *wasFound = TRUE;
        EdgeList* next = edges->next;
        free(((NodeList*)edges->leftNode)->name);
        free(edges->leftNode);
        free(((NodeList*)edges->rightNode)->name);
        free(edges->rightNode);
        free(edges);
        return next;
    }
    edges->next = removeEdgeRecursive(edges->next, leftNodeName, rightNodeName, weight, wasFound);
    return edges;
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
            saved->edgesA = addEdgeRecursive(saved->edgesA, leftNodeName, rightNodeName, weight, &repeated);
            if (repeated == TRUE) {
                return EDGE_ALREADY_EXISTS;              // Edge already exists
            }
        } else {
            saved->edgesA = createEdge(leftNodeName, rightNodeName, weight);
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

