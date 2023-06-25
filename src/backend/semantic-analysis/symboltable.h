#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include "../domain-specific/graphs.h"

typedef struct SymbolTableVariable {
    NodeList *nodesA;
    EdgeList *edgesA;
} sm_variable;

typedef enum Errors {
    NO_ERRORS,
    GRAPH_NOT_EXISTS,
    GRAPH_ALREADY_EXISTS,
    NODE_NOT_EXISTS,
    NODE_ALREADY_EXISTS,
    EDGE_NOT_EXISTS,
    EDGE_ALREADY_EXISTS
} Errors;

void init_symbol_table(void);

void free_symbol_table(void);

int symbol_table_exists(char *name);

int symbol_table_putGraph(char *name);

int symbol_table_nodeExists(char * graphName, char * name);

int symbol_table_addNode(char *graphName, char *name);

int symbol_table_removeNode(char *graphName, char *name);

int symbol_table_addEdge(char *graphName, char *leftNodeName, char* rightNodeName, int weight);

int symbol_table_removeEdge(char *graphName, char *leftNodeName, char* rightNodeName, int weight);

#endif