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
int ProgramGrammarAction(const int value) {
	LogDebug("\tProgramGrammarAction(%d)", value);
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
	state.result = value;
	return value;
}

int Ack() {
	return 0;
}

int AdditionExpressionGrammarAction(const int leftValue, const int rightValue) {
	LogDebug("\tAdditionExpressionGrammarAction(%d, %d)", leftValue, rightValue);
	return Add(leftValue, rightValue);
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

EdgeList * EdgeGrammarAction(EdgeList * edge) {
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

Graph * CreateBipartiteCompleteBlockGrammarAction(char * center, NodeList * list) {
	Graph * graph = calloc(1, sizeof(Graph));
	graph->bipartiteCompleteGraph.center = center;
	graph->bipartiteCompleteGraph.nodeList = list;
	return graph;
}

GraphList * CreateGraphGrammarAction(char * name, Graph * graph, GraphType type) {
	GraphList * graphList = calloc(1, sizeof(GraphList));
	graphList->graph = graph;
	graphList->name = name;
	graphList->graphType = type;
	return graphList;
}

GraphList * CreateSimpleGraphGrammarAction(char * name) {
	return CreateGraphGrammarAction(name, NULL, SIMPLE);
}

GraphList * CreateCycleGraphGrammarAction(char * name, Graph * graph) {
	return CreateComplexGraphGrammarAction(name, graph, CYCLE);
}

GraphList * CreateWheelGraphGrammarAction(char * name, Graph * graph) {
	return CreateComplexGraphGrammarAction(name, graph, WHEEL);
}

GraphList * CreateStarGraphGrammarAction(char * name, Graph * graph) {
	return CreateComplexGraphGrammarAction(name, graph, STAR);
}

GraphList * CreateCompleteGraphGrammarAction(char * name, Graph * graph) {
	return CreateComplexGraphGrammarAction(name, graph, COMPLETE);
}

GraphList * CreateBipartiteCompleteGraphGrammarAction(char * name, Graph * graph) {
	return CreateComplexGraphGrammarAction(name, graph, BIPARTITE_COMPLETE);
}

BlockList * CreateActionBlockGrammarAction(char * name, Block * block, BlockType type) {
	BlockList * blockList = calloc(1, sizeof(BlockList));
	blockList->graphName = name;
	blockList->block = block;
	blockList->type = type;
	return blockList;
}

BlockList * CreateAddBlockGrammarAction(char * name, Block * block) {
	return CreateActionBlockGrammarAction(name, block, ADD);
}

BlockList * CreateRemoveBlockGrammarAction(char * name, Block * block) {
	return CreateActionBlockGrammarAction(name, block, REMOVE);
}

BlockList * CreateApplyBlockGrammarAction(char * name, Block * block) {
	return CreateActionBlockGrammarAction(name, block, APPLY);
}

BlockList * AppendBlockGrammarAction(BlockList * list, BlockList * block) {
	block->next = list;
	return block;
}

BlockList * BlockGrammarAction(BlockList * list) {
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
