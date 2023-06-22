#ifndef SYMBOLTABLE_H_
#define SYMBOLTABLE_H_

#include "abstract-syntax-tree.h"

void init_symbol_table(void);

void free_symbol_table(void);

int symbol_table_putGraph(char *name, GraphList *graph);

int symbol_table_getNode(char *graphName, char *nodeName);

#endif