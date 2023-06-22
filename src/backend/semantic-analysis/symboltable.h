#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include "abstract-syntax-tree.h"

typedef struct SymbolTableVariable {
    NodeList *nodesA;
} sm_variable;

void init_symbol_table(void);

void free_symbol_table(void);

int symbol_table_exists(char *name);

int symbol_table_putGraph(char *name);

int symbol_table_addNodes(char *graphName, NodeList *nodes);

int symbol_table_removeNodes(char *graphName, NodeList *nodes);

#endif