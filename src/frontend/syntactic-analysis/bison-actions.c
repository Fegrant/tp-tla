#include "../../backend/domain-specific/calculator.h"
#include "../../backend/support/logger.h"
#include "bison-actions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Implementación de "bison-grammar.h".
 */

/**
* Esta función se ejecuta cada vez que se emite un error de sintaxis.
*/
void yyerror(const char * string) {
	LogError("Mensaje: '%s' debido a '%s' (linea %d).", string, yytext, yylineno);
	LogError("En ASCII es:");
	LogErrorRaw("\t");
	const int length = strlen(yytext);
	for (int i = 0; i < length; ++i) {
		LogErrorRaw("[%d]", yytext[i]);
	}
	LogErrorRaw("\n\n");
}

/**
* Esta acción se corresponde con el no-terminal que representa el símbolo
* inicial de la gramática, y por ende, es el último en ser ejecutado, lo que
* indica que efectivamente el programa de entrada se pudo generar con esta
* gramática, o lo que es lo mismo, que el programa pertenece al lenguaje.
*/
BlockList * ProgramGrammarAction(BlockList * blockList) {
	// vvvvv De la calculadora vvvvv
	// LogDebug("\tProgramGrammarAction(%d)", 0);
	
	/*
	* "state" es una variable global que almacena el estado del compilador,
	* cuyo campo "succeed" indica si la compilación fue o no exitosa, la cual
	* es utilizada en la función "main".
	*/
	state.succeed = true;
	
	/*
	* Por otro lado, "result" contiene el resultado de aplicar el análisis
	* sintáctico mediante Bison, y almacenar el nood raíz del AST construido
	* en esta variable. Para el ejemplo de la calculadora, no hay AST porque
	* la expresión se computa on-the-fly, y es la razón por la cual esta
	* variable es un simple entero, en lugar de un nodo.
	*/
	// vvvvv De la calculadora vvvvv
	// state.result = 0;

	Program * program = calloc(1, sizeof(Program));
	program->actions = blockList;
	state.program = program;
	return blockList;
}

BlockList * AppendProgramGrammarAction (BlockList * declaration, BlockList * blockList) {
	declaration->next = blockList;
	return ProgramGrammarAction(declaration);
}

// int Ack() {
// 	return 0;
// }

// int AdditionExpressionGrammarAction(const int leftValue, const int rightValue) {
// 	LogDebug("\tAdditionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
// 	return Add(leftValue, rightValue);
// }

BlockList * AppendBlockListGrammarAction(BlockList * block, BlockList * list) {
	block->next = list;
	return block;
}

BlockList * BlockListGrammarAction(BlockList * blockList) {
	return blockList;
}

EdgeList * CreateWeightedEdgeGrammarAction(char * leftNode, char * rightNode, int weight) {
	EdgeList * edge = calloc(1, sizeof(EdgeList));
	edge->leftNode = leftNode;
	edge->rightNode = rightNode;
	edge->weight = weight;
	return edge;
}

EdgeList * CreateEdgeGrammarAction(char * leftNode, char * rightNode) {
	return CreateWeightedEdgeGrammarAction(leftNode, rightNode, 1);
}

EdgeList * AppendEdgeGrammarAction(EdgeList * edge, EdgeList * list) {
	edge->next = list;
	return edge;
} 

EdgeList * EdgeListGrammarAction(EdgeList * edge) {
	return edge;
}

NodeList * CreateNodeGrammarAction(char * name) {
	NodeList * node = calloc(1, sizeof(NodeList));
	node->name = name;
	return node;
}

NodeList * AppendNodeGrammarAction(NodeList * node, NodeList * list) {
	node->next = list;
	return node;
}

AddRemoveInstructionList * AppendAddRemoveInstructionListGrammarAction(AddRemoveInstructionList * instruction, AddRemoveInstructionList * list) {
	instruction->next = list;
	return instruction;
}

AddRemoveInstructionList * AddRemoveInstructionListGrammarAction(AddRemoveInstructionList * list) {
	return list;
}

AddRemoveInstructionList * AddRemoveEdgeListGrammarAction(EdgeList * edgeList) {
	AddRemoveInstruction * instruction = calloc(1, sizeof(AddRemoveInstruction));
	instruction = edgeList;
	AddRemoveInstructionList * instructionList = calloc(1, sizeof(AddRemoveInstructionList));
	instructionList->addRemoveInstruction = instruction;
	instructionList->instructionType = EDGE_LIST;
	return instructionList;
}

AddRemoveInstructionList * AddRemoveNodeListGrammarAction(NodeList * nodeList) {
	AddRemoveInstruction * instruction = calloc(1, sizeof(AddRemoveInstruction));
	instruction = nodeList;
	AddRemoveInstructionList * instructionList = calloc(1, sizeof(AddRemoveInstructionList));
	instructionList->addRemoveInstruction = instruction;
	instructionList->instructionType = NODE_LIST;
	return instructionList;
}

NodeList * NodeListGrammarAction(NodeList * list) {
	return list;
}

Graph * CreateCycleBlockGrammarAction(NodeList * list) {
	Graph * graph = calloc(1, sizeof(Graph));
	graph->cycleGraph.nodeList = list;
	return graph;
}

Graph * CreateCompleteBlockGrammarAction(NodeList * list) {
	Graph * graph = calloc(1, sizeof(Graph));
	graph->completeGraph.nodeList = list;
	return graph;
}

Graph * CreateWheelBlockGrammarAction(char * center, NodeList * list) {
	Graph * graph = calloc(1, sizeof(Graph));
	graph->wheelGraph.center = center;
	graph->wheelGraph.nodeList = list;
	return graph;
}

Graph * CreateBipartiteCompleteBlockGrammarAction(NodeList * groupA, NodeList * groupB) {
	Graph * graph = calloc(1, sizeof(Graph));
	graph->bipartiteCompleteGraph.groupA = groupA;
	graph->bipartiteCompleteGraph.groupB = groupB;
	return graph;
}

BlockList * CreateGraphGrammarAction(char * name, Graph * graph, GraphType type) {
	LogDebug("\tCreateGraphGrammarAction(%s) [type=%d]", name, type);
	GraphList * graphList = calloc(1, sizeof(GraphList));
	graphList->graph = graph;
	graphList->graphType = type;
	BlockList * block = calloc(1, sizeof(BlockList));
	block->block = graphList;
	block->graphName = name;
	block->type = GRAPH;
	return block;
}

BlockList * CreateSimpleGraphGrammarAction(char * name) {
	return CreateGraphGrammarAction(name, NULL, SIMPLE);
}

BlockList * CreateCycleGraphGrammarAction(char * name, Graph * graph) {
	return CreateGraphGrammarAction(name, graph, CYCLE);
}

BlockList * CreateWheelGraphGrammarAction(char * name, Graph * graph) {
	return CreateGraphGrammarAction(name, graph, WHEEL);
}

BlockList * CreateStarGraphGrammarAction(char * name, Graph * graph) {
	return CreateGraphGrammarAction(name, graph, STAR);
}

BlockList * CreateCompleteGraphGrammarAction(char * name, Graph * graph) {
	return CreateGraphGrammarAction(name, graph, COMPLETE);
}

BlockList * CreateBipartiteCompleteGraphGrammarAction(char * name, Graph * graph) {
	return CreateGraphGrammarAction(name, graph, BIPARTITE_COMPLETE);
}

BlockList * CreateActionBlockGrammarAction(char * name, Block * block, BlockType type) {
	BlockList * blockList = calloc(1, sizeof(BlockList));
	blockList->graphName = name;
	blockList->block = block;
	blockList->type = type;
	return blockList;
}

BlockList * CreateAddBlockGrammarAction(char * name, AddRemoveInstructionList * block) {
	return CreateActionBlockGrammarAction(name, (Block*) block, ADD_BLOCK);
}

BlockList * CreateRemoveBlockGrammarAction(char * name, AddRemoveInstructionList * block) {
	return CreateActionBlockGrammarAction(name, (Block*) block, REMOVE_BLOCK);
}

BlockList * CreateApplyBlockGrammarAction(char * name, ApplyInstructionList * block) {
	return CreateActionBlockGrammarAction(name, (Block*) block, APPLY_BLOCK);
}

BlockList * AppendBlockGrammarAction(BlockList * block, BlockList * list) {
	block->next = list;
	return block;
}

BlockList * BlockGrammarAction(BlockList * list) {
	return list;
}

ColorList * AppendColorListGrammarAction(ColorList * colorNodes, ColorList * list) {
	colorNodes->next = list;
	return colorNodes;
}

ColorList * ColorListGrammarAction(ColorList * colorList) {
	return colorList;
}

ColorList * CreateColorListGrammarAction(char * rgb, NodeList * nodes) {
	ColorList * colorList = calloc(1, sizeof(ColorList));
	colorList->rgb = rgb;
	colorList->nodes = nodes;
	return colorList;
}

ApplyInstructionList * CreateColorsBlockGrammarAction(ColorList * colorList) {
	ApplyInstructionList * instructionList = calloc(1, sizeof(ApplyInstructionList));
	instructionList->instructionType = COLORS;
	instructionList->applyInstruction = colorList;
	return instructionList;
}

ApplyInstructionList * CreateTerminalInstruction(ApplyInstructionType type) {
	ApplyInstructionList * instructionList = calloc(1, sizeof(ApplyInstructionList));
	instructionList->instructionType = type;
	return instructionList;
}

ApplyInstructionList * BfsBlockGrammarAction(char * from, char * to) {
	ApplyInstruction * instruction = calloc(1, sizeof(ApplyInstruction));
	instruction->bfs.from = from;
	instruction->bfs.to = to;
	ApplyInstructionList * instructionList = calloc(1, sizeof(ApplyInstructionList));
	instructionList->applyInstruction = instruction;
	instructionList->instructionType = BFS_TYPE;
	return instructionList;
}

ApplyInstructionList * DfsBlockGrammarAction(char * from, char * to) {
	ApplyInstruction * instruction = calloc(1, sizeof(ApplyInstruction));
	instruction->dfs.from = from;
	instruction->dfs.to = to;
	ApplyInstructionList * instructionList = calloc(1, sizeof(ApplyInstructionList));
	instructionList->applyInstruction = instruction;
	instructionList->instructionType = DFS_TYPE;
	return instructionList;
}

ApplyInstructionList * CreateFindCutGrammarActions() {
	return CreateTerminalInstruction(FIND_CUT_NODES);
}

ApplyInstructionList * CreateDeleteGrammarAction() {
	return CreateTerminalInstruction(DELETE_CUT_NODES);
}

ApplyInstructionList * CreateMstGrammarAction() {
	return CreateTerminalInstruction(MST_TYPE);
}

ApplyInstructionList * ApplyInstructionListGrammarAction(ApplyInstructionList * instruction, char * outputFile) {
	instruction->outputFile = outputFile;
	return instruction;
}

ApplyInstructionList * AppendApplyBlockGrammarAction (ApplyInstructionList * instruction, ApplyInstructionList * list) {
	instruction->next=list;
	return instruction;
}

ApplyInstructionList * ApplyBlockGrammarAction (ApplyInstructionList * list) {
	return list;
}

// expression: expression[left] ADD expression[right] { $$ = AdditionExpressionGrammarAction($left, $right); }

// int InstructionBlockGrammarAction(int instruction, int block) {
// 	LogDebug("InstructionBlockGrammarAction ");
// 	int toReturn = malloc(sizeof(Block));
// 	toReturn->type = INSTRUCTION_BLOCK;
// 	toReturn->block = block;
// 	toReturn->instruction = instruction;
// 	return toReturn;
// }

// int BlockGrammarAction(int instruction) {
// 	LogDebug("BlockGrammarAction ");
// 	Block * toReturn = malloc(sizeof(Block));
// 	toReturn->type = INSTRUCTION;
// 	toReturn->block = NULL;
// 	toReturn->instruction = instruction;
// 	return toReturn;
// }
