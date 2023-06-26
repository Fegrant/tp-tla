#include "graphs.h"
#include <stdlib.h>
#include <string.h>

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
    char * leftNode = calloc(1, sizeof(char) * (strlen(leftNodeName) + 1));
    char * rightNode = calloc(1, sizeof(char) * (strlen(rightNodeName) + 1));    

    strcpy(leftNode, leftNodeName);
    strcpy(rightNode, rightNodeName);

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
    
    char * leftNode = edges->leftNode;
    char * rightNode = edges->rightNode;

    int leftCmp = strcmp(leftNode, leftNodeName);
    int rightCmp = strcmp(rightNode, rightNodeName);

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
    char *leftNode = edges->leftNode;
    char *rightNode = edges->rightNode;

    int leftCmp = strcmp(leftNode, leftNodeName);
    int rightCmp = strcmp(rightNode, rightNodeName);

    if (edges == NULL || leftCmp > 0 || (leftCmp == 0 && rightCmp > 0)) {
        *wasFound = FALSE;
        return edges;
    }
    if (leftCmp == 0 && rightCmp == 0) {
        *wasFound = TRUE;
        EdgeList* next = edges->next;
        free(edges->leftNode);
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