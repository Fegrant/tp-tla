#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../../backend/semantic-analysis/abstract-syntax-tree.h"

/**
 * Se definen las acciones a ejecutar sobre cada regla de producción de la
 * gramática. El objetivo de cada acción debe ser el de construir el nodo
 * adecuado que almacene la información requerida en el árbol de sintaxis
 * abstracta (i.e., el AST).
 */

// Programa.
BlockList * ProgramGrammarAction(BlockList * blockList);
BlockList * AppendProgramGrammarAction (BlockList * declaration, BlockList * blockList);

// Added blocks
BlockList * AppendBlockListGrammarAction(BlockList * block, BlockList * list);
BlockList * BlockListGrammarAction(BlockList * blockList);
EdgeList * CreateWeightedEdgeGrammarAction(char * leftNode, char * rightNode, int weight);
EdgeList * CreateEdgeGrammarAction(char * leftNode, char * rightNode);
EdgeList * AppendEdgeGrammarAction(EdgeList * edge, EdgeList * list);
EdgeList * EdgeListGrammarAction(EdgeList * edge);
NodeList * CreateNodeGrammarAction(char * name);
NodeList * AppendNodeGrammarAction(NodeList * node, NodeList * list);
NodeList * NodeListGrammarAction(NodeList * list);
Graph * CreateCycleBlockGrammarAction(NodeList * list);
Graph * CreateCompleteBlockGrammarAction(NodeList * list);
Graph * CreateWheelBlockGrammarAction(char * center, NodeList * list);
Graph * CreateBipartiteCompleteBlockGrammarAction(NodeList * groupA, NodeList * groupB);
BlockList * CreateGraphGrammarAction(char * name, Graph * graph, GraphType type);
BlockList * CreateSimpleGraphGrammarAction(char * name);
BlockList * CreateCycleGraphGrammarAction(char * name, Graph * graph);
BlockList * CreateWheelGraphGrammarAction(char * name, Graph * graph);
BlockList * CreateStarGraphGrammarAction(char * name, Graph * graph);
BlockList * CreateCompleteGraphGrammarAction(char * name, Graph * graph);
BlockList * CreateBipartiteCompleteGraphGrammarAction(char * name, Graph * graph);
BlockList * CreateActionBlockGrammarAction(char * name, Block * block, BlockType type);
BlockList * CreateAddBlockGrammarAction(char * name, AddRemoveInstructionList * block);
BlockList * CreateRemoveBlockGrammarAction(char * name, AddRemoveInstructionList * block);
BlockList * CreateApplyBlockGrammarAction(char * name, ApplyInstructionList * block);
BlockList * AppendBlockGrammarAction(BlockList * list, BlockList * block);
BlockList * BlockGrammarAction(BlockList * list);
ColorList * AppendColorListGrammarAction(ColorList * colorNodes, ColorList * list);
ColorList * ColorListGrammarAction(ColorList * colorList);
ColorList * CreateColorListGrammarAction(char rgb[7], NodeList * nodes);
ApplyInstructionList * CreateColorsBlockGrammarAction(ColorList * colorList);
ApplyInstructionList * CreateTerminalInstruction(ApplyInstructionType type);
ApplyInstructionList * BfsBlockGrammarAction(char * from, char * to);
ApplyInstructionList * DfsBlockGrammarAction(char * from, char * to);
ApplyInstructionList * CreateFindCutGrammarActions();
ApplyInstructionList * CreateDeleteGrammarAction();
ApplyInstructionList * CreateMstGrammarAction();
ApplyInstructionList * ApplyInstructionListGrammarAction(ApplyInstructionList * instruction, char * outputFile);
ApplyInstructionList * AppendApplyBlockGrammarAction (ApplyInstructionList * instruction, ApplyInstructionList * list);
ApplyInstructionList * ApplyBlockGrammarAction (ApplyInstructionList * list);

AddRemoveInstructionList * AddRemoveEdgeListGrammarAction(EdgeList * edgeList);
AddRemoveInstructionList * AddRemoveNodeListGrammarAction(NodeList * nodeList);

AddRemoveInstructionList * AppendAddRemoveInstructionListGrammarAction(AddRemoveInstructionList * instruction, AddRemoveInstructionList * list);
AddRemoveInstructionList * AddRemoveInstructionListGrammarAction(AddRemoveInstructionList * list);

#endif
