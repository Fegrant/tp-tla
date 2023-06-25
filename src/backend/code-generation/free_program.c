#include <stdlib.h>
#include "generator.h"

void freeGraph(GraphList * graph) {
    switch(graph->graphType) {
        case CYCLE:
            freeNodeList(graph->graph->cycleGraph.nodeList);
            break;
        case WHEEL:
            if (graph->graph->wheelGraph.center != NULL) {
                free(graph->graph->wheelGraph.center);
            }
            freeNodeList(graph->graph->wheelGraph.nodeList);
            break;
        case STAR:
            if (graph->graph->starGraph.center != NULL) {
                free(graph->graph->starGraph.center);
            }
            freeNodeList(graph->graph->starGraph.nodeList);      
            break;
        case COMPLETE:
            freeNodeList(graph->graph->completeGraph.nodeList);
            break;
        case BIPARTITE_COMPLETE:
            freeNodeList(graph->graph->bipartiteCompleteGraph.groupA);
            freeNodeList(graph->graph->bipartiteCompleteGraph.groupB);
            break;
        case SIMPLE:
        default:
    }
    free(graph->graph);
    free(graph);
    return;
}

void freeNodeList(NodeList * list) {
    if(list != NULL) {
        freeNodeList(list->next);
        if (list->name != NULL) {
            free(list->name);
        }
        free(list);
    }
    return;
}

void freeEdgeList(EdgeList * list) {
    if(list != NULL) {
        freeEdgeList(list->next);
        if (list->leftNode != NULL) {
            free(list->leftNode);
        }
        if (list->rightNode != NULL) {
            free(list->rightNode);
        }
        free(list);
    }
    return;
}

void freeAddRemove(AddRemoveInstructionList * list) {
    if (list != NULL) {
        freeAddRemove(list->next);
        switch (list->instructionType) {
        case NODE_LIST:
            freeNodeList((NodeList *) list->addRemoveInstruction);
            break;
        case EDGE_LIST:
            freeEdgeList((EdgeList *) list->addRemoveInstruction);
            break;
        default:
            break;
        }
        free(list);
    }
}

void freeApply(ApplyInstructionList * list) {
    if (list != NULL) {
        freeApply(list->next);
        if (list->outputFile != NULL) {
            free(list->outputFile);
        }
        switch (list->instructionType) {
        case BFS_TYPE:
            if(list->applyInstruction->bfs.from != NULL) {
                free(list->applyInstruction->bfs.from);
            }
            if(list->applyInstruction->bfs.to != NULL) {
                free(list->applyInstruction->bfs.to);
            }
            free(list->applyInstruction);
            break;
        case DFS_TYPE:
            if(list->applyInstruction->dfs.from != NULL) {
                free(list->applyInstruction->dfs.from);
            }
            if(list->applyInstruction->dfs.to != NULL) {
                free(list->applyInstruction->dfs.to);
            }
            free(list->applyInstruction);
            break;
        case COLORS:
            freeColors((ColorList *) list->applyInstruction);
            break;
        case FIND_CUT_NODES:
            break;
        case DELETE_CUT_NODES:
            break;
        case MST_TYPE:
            break;
        default:
            break;
        }
        free(list);
    }
}

void freeColors(ColorList * list) {
    if (list != NULL) {
        freeColors(list->next);
        if(list->rgb != NULL) {
            free(list->rgb);
        }
        freeNodeList(list->nodes);
        free(list);
    }
    return;
}

void freeOutputGraph(OutputGraphInstruction * instruction) {
    if (instruction != NULL) {
        if (instruction->outputFile != NULL) {
            free(instruction->outputFile);
        }
        free(instruction);
    }
    return;
}
