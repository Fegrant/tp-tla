#include "../support/logger.h"
#include "free_program.h"
#include "generator.h"
#include <stdlib.h>

/**
 * Implementación de "generator.h".
 */

void generateSetup();

void generateGraph(char *graphName, GraphList *graph);

void generateAdd(char *graphName, AddRemoveInstructionList *addList);

void generateRemove(char *graphName, AddRemoveInstructionList *addList);

void generateApply(char *graphName, ApplyInstructionList *applyList);

FILE * fd;

int fileCounter = 0;

void Generator(Program *program) {
	const char *filename = "output.py";
	fd = fopen(filename, "w+");
	generateSetup();

	BlockList *action = program->actions;
	BlockList *next;

	while (action != NULL)
	{
		next = action->next;
		switch (action->type)
		{
		case GRAPH:
			generateGraph(action->graphName, (GraphList *) action->block);
			freeGraph((GraphList *) action->block);
			break;
		case ADD_BLOCK:
			generateAdd(action->graphName, (AddRemoveInstructionList *) action->block);
			freeAddRemove((AddRemoveInstructionList *) action->block);
			break;
		case REMOVE_BLOCK:
			generateRemove(action->graphName, (AddRemoveInstructionList *) action->block);
			freeAddRemove((AddRemoveInstructionList *) action->block);
			break;
		case APPLY_BLOCK:
			generateApply(action->graphName, (ApplyInstructionList *) action->block);
			freeApply((ApplyInstructionList *) action->block);
			break;
		default:
			break;
		}
		if (action->graphName != NULL) {
			free(action->graphName);
		}
		free(action);
		action = next;
	}

	free(program);
}

void generateSetup() {
	fprintf(fd, "import matplotlib.pyplot as plt\n");
	fprintf(fd, "import networkx as _nx\n\n");
	fprintf(fd, "import os\n\n");
	fprintf(fd, "if not os.path.exists('output'):\n");
	fprintf(fd, "\tos.makedirs('output')\n\n");
}

void generateGraph(char *graphName, GraphList * graph) {
	fprintf(fd, "%s = _nx.Graph()\n", graphName);

	NodeList *a = NULL;
	char *center = NULL;
	NodeList *b = NULL;

	switch (graph->graphType) {
		case CYCLE:
			a = graph->graph->cycleGraph.nodeList;
			break;
		case WHEEL:
			a = graph->graph->wheelGraph.nodeList;
			center = graph->graph->wheelGraph.center;
			break;
		case STAR:
			a = graph->graph->starGraph.nodeList;
			center = graph->graph->starGraph.center;
			break;
		case COMPLETE:
			a = graph->graph->completeGraph.nodeList;
			break;
		case BIPARTITE_COMPLETE:
			a = graph->graph->bipartiteCompleteGraph.groupA;
			b = graph->graph->bipartiteCompleteGraph.groupB;
			break;
		case SIMPLE:
		default:
			break;
	}

	NodeList *aux;
	
	if (graph->graphType != SIMPLE) {
		fprintf(fd, "%s.add_nodes_from([", graphName);
	}

	for (NodeList *aux = a; aux != NULL; aux = aux->next) {
		fprintf(fd, "'%s', ", aux->name);
	}

	if (center != NULL) {
		fprintf(fd, "'%s', ", center);
	}

	for (NodeList *aux = b; aux != NULL; aux = aux->next) {
		fprintf(fd, "'%s', ", aux->name);
	}

	if (graph->graphType != SIMPLE) {
		fprintf(fd, "])\n");
		fprintf(fd, "%s.add_weighted_edges_from([", graphName);
	}

	switch (graph->graphType) {
		case STAR:
		case WHEEL:
			for (aux = a; aux; aux = aux->next) {
				fprintf(fd, "('%s', '%s', %d), ", aux->name, center, 1);
			}
			if (graph->graphType == STAR)
				break;
		case CYCLE:
			for (aux = a; aux->next != NULL; aux = aux->next) {
				fprintf(fd, "('%s', '%s', %d), ", aux->name, aux->next->name, 1);
			}
			fprintf(fd, "('%s', '%s', %d), ", aux->name, a->name, 1);
			break;
		case COMPLETE:
			for (aux = a; aux; aux = aux->next) {
				for (NodeList *aux2 = aux->next; aux2; aux2 = aux2->next) {
					fprintf(fd, "('%s', '%s', %d), ", aux->name, aux2->name, 1);
				}
			}
			break;
		case SIMPLE:
		default:
			break;
	}

	if (graph->graphType != SIMPLE) {
		fprintf(fd, "])\n");
	}
}

void generateAdd(char *graphName, AddRemoveInstructionList *addList) {
	for (AddRemoveInstructionList * aux = addList; aux != NULL; aux = aux->next) {
		if (aux->instructionType == NODE_LIST) {
			fprintf(fd, "%s.add_nodes_from([", graphName);
			for (NodeList *node = (NodeList *)aux->addRemoveInstruction; node; node = node->next) {
				fprintf(fd, "'%s', ", node->name);
			}
			fprintf(fd, "])\n");
		} else if (aux->instructionType == EDGE_LIST) {
			fprintf(fd, "%s.add_weighted_edges_from([", graphName);
			for (EdgeList *aux2 = (EdgeList *)aux->addRemoveInstruction; aux2; aux2 = aux2->next) {
				fprintf(fd, "('%s', '%s', %d), ", ((NodeList*)aux2->leftNode)->name, ((NodeList*)aux2->rightNode)->name, aux2->weight);
			}
			fprintf(fd, "])\n");
		}
	}	
}

void generateRemove(char *graphName, AddRemoveInstructionList *addList) {
	for (AddRemoveInstructionList *aux = addList; aux; aux = aux->next) {
		if (aux->instructionType == NODE_LIST) {
			fprintf(fd, "%s.remove_nodes_from([", graphName);
			for (NodeList *node = (NodeList *)aux->addRemoveInstruction; node; node = node->next) {
				fprintf(fd, "'%s', ", node->name);
			}
			fprintf(fd, "])\n");
		} else if (aux->instructionType == EDGE_LIST) {
			fprintf(fd, "%s.remove_edges_from([", graphName);
			for (EdgeList *aux2 = (EdgeList *)aux->addRemoveInstruction; aux2; aux2 = aux2->next) {
				fprintf(fd, "('%s', '%s'), ", ((NodeList*)aux2->leftNode)->name, ((NodeList*)aux2->rightNode)->name);
			}
			fprintf(fd, "])\n");
		}
	}
}

static void printNodeLabels(char* graphName) {
	fprintf(fd, "%s_node_labels = {}\n", graphName);
	fprintf(fd, "for node in %s.nodes:\n", graphName);
	fprintf(fd, "\t%s_node_labels[node] = node\n", graphName);
}

static void printEdgesColors(char* graphName) {
	fprintf(fd, "%s_edges_colors = []\n", graphName);
	fprintf(fd, "for edge in %s.edges:\n", graphName);
	fprintf(fd, "\tif edge in %s_bfs_edges:\n", graphName);
	fprintf(fd, "\t\t%s_edges_colors.append('red')\n", graphName);
	fprintf(fd, "\telse:\n");
	fprintf(fd, "\t\t%s_edges_colors.append('black')\n", graphName);
}

void generateApply(char *graphName, ApplyInstructionList *applyList) {
	fprintf(fd, "\n");
	fprintf(fd, "%s_node_colors = None\n\n", graphName);
	for (ApplyInstructionList *instruction = applyList; instruction; instruction = instruction->next) {
		char *varName = graphName;
		int edgeColors = 0;
		int nodeColors = 1;
		int printsGraph = 1;
		switch (instruction->instructionType)
		{
		case BFS_TYPE: ;
			BfsBlock *bfs = (BfsBlock *)instruction->applyInstruction;
			fprintf(fd, "%s_bfs_tree = _nx.bfs_tree(%s, '%s')\n", graphName, graphName, bfs->from);
			fprintf(fd, "%s_bfs_edges = [tuple(sorted(edge)) for edge in _nx.utils.pairwise(_nx.shortest_path(%s_bfs_tree, '%s', '%s'))]\n", graphName, graphName, bfs->from, bfs->to);
			printNodeLabels(graphName);
			printEdgesColors(graphName);
			edgeColors = 1;
			break;
		case DFS_TYPE: ;
			DfsBlock *dfs = (DfsBlock *)instruction->applyInstruction;
			fprintf(fd, "%s_dfs_tree = _nx.dfs_tree(%s, '%s')\n", graphName, graphName, dfs->from);
			fprintf(fd, "%s_dfs_edges = [tuple(sorted(edge)) for edge in _nx.utils.pairwise(_nx.shortest_path(%s_dfs_tree, '%s', '%s'))]\n", graphName, graphName, dfs->from, dfs->to);
			printNodeLabels(graphName);
			printEdgesColors(graphName);
			edgeColors = 1;
			break;
		case COLORS: ;
			ColorList *colors = (ColorList *)instruction->applyInstruction;
			fprintf(fd, "%s_node_colors = []\n", graphName);
			fprintf(fd, "for node in %s.nodes:\n", graphName);
			for (ColorList *aux = colors; aux; aux = aux->next) {
				for (NodeList *node = aux->nodes; node; node = node->next) {
					fprintf(fd, "\tif node == '%s':\n", node->name);
					fprintf(fd, "\t\t%s_node_colors.append('%s')\n", graphName, aux->rgb);
				}
			}
			printsGraph = 0;
			break;
		case FIND_CUT_NODES:
			printNodeLabels(graphName);
			fprintf(fd, "%s_cut = _nx.minimum_node_cut(%s)\n"
						"%s_node_colors = ['red' if node in %s_cut else 'white' for node in %s.nodes]\n", graphName, graphName, graphName, graphName, graphName);
			fprintf(fd, "%s_edges_colors = []\n", graphName);
			break;
		case DELETE_CUT_NODES:
			fprintf(fd, "%s_cut = _nx.minimum_node_cut(%s)\n"
						"for node in %s_cut:\n"
						"\t%s.remove_node(node)\n", graphName, graphName, graphName, graphName);
			printNodeLabels(graphName);
			fprintf(fd, "%s_node_colors = None\n", graphName);
			break;
		case MST_TYPE:
			fprintf(fd, "%s_mst = _nx.minimum_spanning_tree(%s)\n", graphName, graphName);
			fprintf(fd, "%s_mst_edges = [tuple(sorted([edge[0], edge[1]])) for edge in %s_mst.edges(data=True)]\n", graphName, graphName);
			printNodeLabels(graphName);
			printEdgesColors(graphName);
			edgeColors = 1;
			break;
		default:
			break;
		}
		if (printsGraph) {
			fprintf(fd, "%s_pos = _nx.spring_layout(%s)\n", graphName, graphName);
			if (edgeColors) {
				fprintf(fd, "_nx.draw(%s, edge_color=%s_edges_colors, node_color='white', pos=%s_pos)\n", graphName, graphName, graphName);
			} else {
				fprintf(fd, "_nx.draw(%s, node_color='white', pos=%s_pos)\n", graphName, graphName);
			}
			fprintf(fd, "_nx.draw_networkx_edge_labels(%s, %s_pos, edge_labels=_nx.get_edge_attributes(%s, 'weight'))\n", graphName, graphName, graphName);
			fprintf(fd, "%s_labels = _nx.draw_networkx_labels(%s, %s_pos, labels=%s_node_labels, font_weight='bold', bbox=dict(facecolor='white', edgecolor='black', boxstyle='round,pad=0.4'))\n", graphName, graphName, graphName, graphName);
			fprintf(fd, "if %s_node_colors is not None:\n", graphName);
			fprintf(fd, "\tfor t, c in zip(%s_labels.values(), %s_node_colors):\n", graphName, graphName);
			fprintf(fd, "\t\tt.set_backgroundcolor(c)\n");
			char * outputFile = instruction->outputFile;
			if (outputFile == NULL) {
				outputFile = graphName;
			}
			fprintf(fd, "plt.savefig('output/%s.png')\n", outputFile);
			fprintf(fd, "plt.clf()\n\n");
			fileCounter++;
		}
	}
}

