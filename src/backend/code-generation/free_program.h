#ifndef FREE_PROGRAM_H
#define FREE_PROGRAM_H

#include <stdlib.h>
#include "generator.h"

void freeGraph(GraphList * graph);
void freeNodeList(NodeList * list);
void freeEdgeList(EdgeList * list);
void freeAddRemove(AddRemoveInstructionList * list);
void freeApply(ApplyInstructionList * list);
void freeColors(ColorList * list);
void freeOutputGraph(OutputGraphInstruction * instruction);

#endif