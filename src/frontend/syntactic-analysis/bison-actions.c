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

static int programSuccess = true;

/**
* Esta acción se corresponde con el no-terminal que representa el símbolo
* inicial de la gramática, y por ende, es el último en ser ejecutado, lo que
* indica que efectivamente el programa de entrada se pudo generar con esta
* gramática, o lo que es lo mismo, que el programa pertenece al lenguaje.
*/
BlockList * ProgramGrammarAction(BlockList * blockList) {
	// vvvvv De la calculadora vvvvv
	// LogDebug("\tProgramGrammarAction(%d)", 0);
	LogDebug("\tProgramGrammarAction(graph:%s)", blockList->graphName);
	
	/*
	* "state" es una variable global que almacena el estado del compilador,
	* cuyo campo "succeed" indica si la compilación fue o no exitosa, la cual
	* es utilizada en la función "main".
	*/
	state.succeed = programSuccess;
	
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
	LogDebug("\tAppendProgramGrammarAction(graph:%s, top_of_list:%s)", declaration->graphName, blockList->graphName);
	declaration->next = blockList;
	return ProgramGrammarAction(declaration);
}

BlockList * AppendBlockListGrammarAction(BlockList * block, BlockList * list) {
	LogDebug("\tAppendBlockListGrammarAction(graph:%s, top_of_list:%s)", block->graphName, list->graphName);
	block->next = list;
	return block;
}

BlockList * BlockListGrammarAction(BlockList * blockList) {
	LogDebug("\tBlockListGrammarAction(graph:%s)", blockList->graphName);
	return blockList;
}

EdgeList * CreateWeightedEdgeGrammarAction(char * leftNode, char * rightNode, int weight) {
	LogDebug("\tCreateWeightedEdgeGrammarAction(%s, %s, %d)", leftNode, rightNode, weight);
	EdgeList * edge = calloc(1, sizeof(EdgeList));
	if (strcmp(leftNode, rightNode) == 0) {
		LogError("No puede existir un lazo en el nodo '%s'", leftNode);
		programSuccess = false;
	}
	edge->leftNode = leftNode;
	edge->rightNode = rightNode;
	edge->weight = weight;
	return edge;
}

EdgeList * CreateEdgeGrammarAction(char * leftNode, char * rightNode) {
	LogDebug("\tCreateEdgeGrammarAction(%s, %s)", leftNode, rightNode);
	return CreateWeightedEdgeGrammarAction(leftNode, rightNode, 1);
}

EdgeList * AppendEdgeGrammarAction(EdgeList * edge, EdgeList * list) {
	LogDebug("\tAppendEdgeGrammarAction(edge:(%s, %s), top_of_list:(%s, %s))", edge->leftNode, edge->rightNode, list->leftNode, list->rightNode);
	edge->next = list;
	return edge;
} 

EdgeList * EdgeListGrammarAction(EdgeList * edge) {
	LogDebug("\tEdgeListGrammarAction(%s, %s)", edge->leftNode, edge->rightNode);
	return edge;
}

NodeList * CreateNodeGrammarAction(char * name) {
	LogDebug("\tCreateNodeGrammarAction(%s)", name);
	NodeList * node = calloc(1, sizeof(NodeList));
	node->name = name;
	return node;
}

NodeList * AppendNodeGrammarAction(NodeList * node, NodeList * list) {
	LogDebug("\tAppendNodeGrammarAction(node:%s, top_of_list:%s)", node->name, list->name);
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
	instruction = (AddRemoveInstruction*)edgeList;
	AddRemoveInstructionList * instructionList = calloc(1, sizeof(AddRemoveInstructionList));
	instructionList->addRemoveInstruction = instruction;
	instructionList->instructionType = EDGE_LIST;
	return instructionList;
}

AddRemoveInstructionList * AddRemoveNodeListGrammarAction(NodeList * nodeList) {
	AddRemoveInstruction * instruction = calloc(1, sizeof(AddRemoveInstruction));
	instruction = (AddRemoveInstruction*)nodeList;
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
	block->block = (Block*)graphList;
	block->graphName = name;
	block->type = GRAPH;
	if (symbol_table_putGraph(name) == GRAPH_ALREADY_EXISTS) {
		LogError("No pueden haber 2 grafos con nombre %s", name);
		programSuccess = false;
	}
	if (graphList->graphType == CYCLE) {
		CycleGraph * c_graph = (CycleGraph*)graph;
		NodeList * last = NULL;
		for (NodeList * node = c_graph->nodeList ; node ; node = node->next) {
			int result = symbol_table_addNode(name, node->name);
			if (result == NODE_ALREADY_EXISTS) {
				LogError("No pueden existir 2 nodos '%s' en el grafo '%s'", node->name, name);
				programSuccess = false;				
			}
			if (last != NULL) {
				result = symbol_table_addEdge(name, last->name, node->name, 1);
				if (result == EDGE_ALREADY_EXISTS) {		// Maybe unreachable, but just in case
					LogError("No pueden existir 2 aristas '%s-%s' en el grafo '%s'", last->name, node->name, name);
					programSuccess = false;
				}
			}
			last = node;
		}
	} else if (graphList->graphType == WHEEL) {
		WheelGraph * w_graph = (WheelGraph*)graph;
		symbol_table_addNode(name, w_graph->center);
		NodeList * last = NULL;
		for (NodeList *node = w_graph->nodeList ; node ; node = node->next) {
			int result = symbol_table_addNode(name, node->name);
			if (result == NODE_ALREADY_EXISTS) {
				LogError("No pueden existir 2 nodos '%s' en el grafo '%s'", node->name, name);
				programSuccess = false;
			}
			result = symbol_table_addEdge(name, w_graph->center, node->name, 1);
			if (result == EDGE_ALREADY_EXISTS) {
				LogError("No pueden existir 2 aristas '%s-%s' en el grafo '%s'", w_graph->center, node->name, name);
				programSuccess = false;
			}
			if (last != NULL) {
				result = symbol_table_addEdge(name, node->name, last->name, 1);
				if (result == EDGE_ALREADY_EXISTS) {		// Maybe unreachable, but just in case
					LogError("No pueden existir 2 aristas '%s-%s' en el grafo '%s'", w_graph->center, node->name, name);
					programSuccess = false;
				}
			}
			last = node;
		}
	} else if (graphList->graphType == STAR) {
		StarGraph * s_graph = (StarGraph*)graphList->graph;
		symbol_table_addNode(name, s_graph->center);
		for (NodeList *node = s_graph->nodeList ; node ; node = node->next) {
			int result = symbol_table_addNode(name, node->name);
			if (result == NODE_ALREADY_EXISTS) {
				LogError("No pueden existir 2 nodos '%s' en el grafo '%s'", node->name, name);
				programSuccess = false;
			}
			result = symbol_table_addEdge(name, s_graph->center, node->name, 1);
			if (result == EDGE_ALREADY_EXISTS) {
				LogError("No pueden existir 2 aristas '%s-%s' en el grafo '%s'", s_graph->center, node->name, name);
				programSuccess = false;
			}
		}
	} else if (graphList->graphType == COMPLETE) {
		CompleteGraph * k_graph = (CompleteGraph*)graphList->graph;
		for (NodeList *node = k_graph->nodeList ; node ; node=node->next) {
			int result = symbol_table_addNode(name, node->name);
			if (result == NODE_ALREADY_EXISTS) {
				LogError("No pueden existir 2 nodos '%s' en el grafo '%s'", node->name, name);
				programSuccess = false;
			}
		}
		for (NodeList *node = k_graph->nodeList ; node ; node=node->next) {
			for (NodeList *connected = node->next ; connected ; connected = connected->next) {
				int result = symbol_table_addEdge(name, node->name, connected->name, 1);
				if (result == EDGE_ALREADY_EXISTS) {
					LogError("No pueden existir 2 aristas '%s-%s' en el grafo '%s'", connected->name, node->name, name);
					programSuccess = false;					
				}
			}
		}
	} else if (graphList->graphType == BIPARTITE_COMPLETE) {
		BipartiteCompleteGraph * b_graph = (BipartiteCompleteGraph*)graphList->graph;
		for (NodeList *node = b_graph->groupA ; node ; node = node->next) {
			int result = symbol_table_addNode(name, node->name);
			if (result == NODE_ALREADY_EXISTS) {
				LogError("No pueden existir 2 nodos '%s' en el grafo '%s'", node->name, name);
				programSuccess = false;
			}
		}
		for (NodeList *node = b_graph->groupB ; node ; node = node->next) {
			int result = symbol_table_addNode(name, node->name);
			if (result == NODE_ALREADY_EXISTS) {
				LogError("No pueden existir 2 nodos '%s' en el grafo '%s'", node->name, name);
				programSuccess = false;
			}
			for (NodeList * groupANode = b_graph->groupA ; groupANode ; groupANode = groupANode->next) {
				result = symbol_table_addEdge(name, node->name, groupANode->name, 1);
				if (result == EDGE_ALREADY_EXISTS) {
					LogError("No pueden existir 2 aristas '%s-%s' en el grafo '%s'", groupANode->name, node->name, name);
					programSuccess = false;
				}
			}
		}
	}
	return block;
}

BlockList * CreateSimpleGraphGrammarAction(char * name) {
	return CreateGraphGrammarAction(name, NULL, SIMPLE);
}

BlockList * CreateCycleGraphGrammarAction(char * name, Graph * graph) {
	BlockList * toReturn = CreateGraphGrammarAction(name, graph, CYCLE);
	return toReturn;
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
	if (!symbol_table_exists(name)) {		// Graph does not exist
		LogError("No existe el grafo '%s' sobre el que se quiere operar", name);
		programSuccess = false;
	}
	switch (type) {
		case ADD_BLOCK: ;
			AddRemoveInstructionList * addNodesEdgesList = (AddRemoveInstructionList *)blockList->block;
			for (AddRemoveInstructionList * instruction = addNodesEdgesList ; instruction ; instruction = instruction->next) {
				if (instruction->instructionType == NODE_LIST) {
					for (NodeList *node = (NodeList*)instruction->addRemoveInstruction ; node ; node = node->next) {
						int result = symbol_table_addNode(name, node->name);
						if (result == NODE_ALREADY_EXISTS) {
							LogError("No pueden existir 2 nodos '%s' en el grafo '%s'", node->name, name);
							programSuccess = false;
						} else if (result == GRAPH_NOT_EXISTS) {
							LogError("El grafo '%s' sobre el que se quiere añadir el nodo '%s' no existe", name, node->name);
							programSuccess = false;
						}
					}
				} else if (instruction->instructionType == EDGE_LIST) {
					for (EdgeList *edge = (EdgeList*)instruction->addRemoveInstruction ; edge ; edge = edge->next) {
						char* leftNode = edge->leftNode;
						char* rightNode = edge->rightNode;
						int result = symbol_table_addEdge(name, leftNode, rightNode, edge->weight);
						if (result == EDGE_ALREADY_EXISTS) {
							LogError("No pueden existir 2 aristas '%s-%s' en el grafo '%s'", leftNode, rightNode, name);
							programSuccess = false;
						} else if (result == NODE_NOT_EXISTS) {
							LogError("Un nodo de la arista '%s-%s' no existe en el grafo '%s'", leftNode, rightNode, name);
							programSuccess = false;
						} else if (result == GRAPH_NOT_EXISTS) {
							LogError("El grafo '%s' sobre el que se quiere añadir la arista '%s-%s' no existe", name, (char*)leftNode, (char*)rightNode);
							programSuccess = false;
						}
					}
				}
			}
			break;
		case REMOVE_BLOCK: ;
			AddRemoveInstructionList * removeNodesEdgesList = (AddRemoveInstructionList *)blockList->block;
			for (AddRemoveInstructionList * instruction = removeNodesEdgesList ; instruction ; instruction = instruction->next) {
				if (instruction->instructionType == NODE_LIST) {
					for (NodeList *node = (NodeList*)instruction->addRemoveInstruction ; node ; node = node->next) {
						int result = symbol_table_removeNode(name, node->name);
						if (result == NODE_NOT_EXISTS) {
							LogError("El nodo a borrar '%s' no existe en el grafo '%s'", node->name, name);
							programSuccess = false;
						} else if (result == GRAPH_NOT_EXISTS) {
							LogError("El grafo '%s' no existe", name);
							programSuccess = false;
						}
					}
				} else if (instruction->instructionType == EDGE_LIST) {
					for (EdgeList *edge = (EdgeList*)instruction->addRemoveInstruction ; edge ; edge = edge->next) {
						char* leftNode = edge->leftNode;
						char* rightNode = edge->rightNode;
						int result = symbol_table_removeEdge(name, leftNode, rightNode, edge->weight);
						if (result == EDGE_NOT_EXISTS) {
							LogError("La arista '%s-%s' a borrar no existe en el grafo '%s'", leftNode, rightNode, name);
							programSuccess = false;
						} else if (result == NODE_NOT_EXISTS) {
							LogError("Un nodo de la arista '%s-%s' no existe en el grafo '%s'", leftNode, rightNode, name);
							programSuccess = false;
						} else if (result == GRAPH_NOT_EXISTS) {
							LogError("El grafo '%s' sobre el que se quiere borrar la arista '%s-%s' no existe", name, leftNode, rightNode);
							programSuccess = false;
						}
					}
				}
			}
			break;
		case APPLY_BLOCK: ;
			ApplyInstructionList * instructionList = (ApplyInstructionList *)blockList->block;
			for (ApplyInstructionList * instruction = instructionList ; instruction ; instruction = instruction->next ) {
				if (instruction->instructionType == DFS_TYPE) {
					char * from = ((DfsBlock*)instruction->applyInstruction)->from;
					int fromResult = symbol_table_nodeExists(name, from);
					if (fromResult == GRAPH_NOT_EXISTS) {
						LogError("El grafo '%s' al que se le quiere aplicar DFS no existe", name);
						programSuccess = false;
					} else if (fromResult == NODE_NOT_EXISTS) {
						LogError("El nodo '%s' del grafo '%s' desde el que se quiere hacer DFS no existe", from, name);
						programSuccess = false;
					}
					char * to = ((DfsBlock*)instruction->applyInstruction)->to;
					int toResult = symbol_table_nodeExists(name, to);
					if (toResult == GRAPH_NOT_EXISTS) {
						LogError("El grafo '%s' al que se le quiere aplicar DFS no existe", name);
						programSuccess = false;
					} else if (toResult == NODE_NOT_EXISTS) {
						LogError("El nodo '%s' del grafo '%s' al que se quiere llegar con DFS no existe", to, name);
						programSuccess = false;
					}
				} else if (instruction->instructionType == BFS_TYPE) {
					char * from = ((BfsBlock*)instruction->applyInstruction)->from;
					int fromResult = symbol_table_nodeExists(name, from);
					if (fromResult == GRAPH_NOT_EXISTS) {
						LogError("El grafo '%s' al que se le quiere aplicar DFS no existe", name);
						programSuccess = false;
					} else if (fromResult == NODE_NOT_EXISTS) {
						LogError("El nodo '%s' del grafo '%s' desde el que se quiere hacer BFS no existe", from, name);
						programSuccess = false;
					}
					char * to = ((BfsBlock*)instruction->applyInstruction)->to;
					int toResult = symbol_table_nodeExists(name, to);
					if (toResult == GRAPH_NOT_EXISTS) {
						LogError("El grafo '%s' al que se le quiere aplicar DFS no existe", name);
						programSuccess = false;
					} else if (toResult == NODE_NOT_EXISTS) {
						LogError("El nodo '%s' del grafo '%s' al que se quiere llegar con BFS no existe", to, name);
						programSuccess = false;
					}
				} else if (instruction->instructionType == COLORS) {
					NodeList *nodes = ((ColorList*)instruction->applyInstruction)->nodes;
					for (NodeList *node = nodes ; node ; node = node->next) {
						int result = symbol_table_nodeExists(name, node->name);
						if(result == NODE_NOT_EXISTS) {
							LogError("El nodo '%s' que se quiere colorear de %s no existe en el grafo %s", node->name, ((ColorList*)instruction->applyInstruction)->rgb, name);
							programSuccess = false;
						}
					}
				}
			}
			break;
		default:
			break;
	}
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
	instructionList->applyInstruction = (ApplyInstruction*)colorList;
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

BlockList * CreateOutputGraphBlockGrammarAction(char *name, char *output) {
	LogDebug("\tCreateOutputGraphBlockGrammarAction(%s, %s)\n", name, output);
	if (!symbol_table_exists(name)) {
		LogError("El grafo '%s' que se quiere exportar no existe", name);
		programSuccess = false;
	}

	BlockList * block = calloc(1, sizeof(BlockList));
	OutputGraphInstruction * instruction = calloc(1, sizeof(OutputGraphInstruction));
	instruction->outputFile = output;
	block->block = (Block *)instruction;
	block->graphName = name;
	block->type = OUTPUT_GRAPH;
	return block;
}
