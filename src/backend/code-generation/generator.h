#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../semantic-analysis/abstract-syntax-tree.h"

void Generator(Program *program);

void freeGraph(GraphList * graph);
void freeNodeList(NodeList * list);
void freeEdgeList(EdgeList * list);
void freeAddRemove(AddRemoveInstructionList * list);
void freeApply(ApplyInstructionList * list);
void freeColors(ColorList * list);

#endif
