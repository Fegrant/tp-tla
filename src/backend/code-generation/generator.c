#include "../support/logger.h"
#include "generator.h"
#include "../semantic-analysis/abstract-syntax-tree.h"

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
	// TODO: Code generation

	const char *filename = "output.js";
	fd = fopen(filename, "w+");

	generateSetup();

	BlockList *action = program->actions;

	while (action != NULL)
	{
		switch (action->type)
		{
		case GRAPH:
			generateGraph(action->graphName, (GraphList *) action->block);
			break;
		case ADD_BLOCK:
			generateAdd(action->graphName, (AddRemoveInstructionList *) action->block);
			break;
		case REMOVE_BLOCK:
			generateRemove(action->graphName, (AddRemoveInstructionList *) action->block);
			break;
		case APPLY_BLOCK:
			generateApply(action->graphName, (ApplyInstructionList *) action->block);
			break;
		default:
			break;
		}
		action = action->next;
	}
	
}

void generateSetup() {
	fprintf(fd, "const cytoscape = require('cytoscape');\n");
	fprintf(fd, "const cytosnap = require('cytosnap');\n");
	fprintf(fd, "cytosnap.use(['cytoscape-dagre', 'cytoscape-cose-bilkent']);\n");
	fprintf(fd, "const cytosnapOptions = {format: 'png', width: 1000, height: 1000, background: 'white'}\n");
	fprintf(fd, "var snap = cytosnap(cytoscape, cytosnapOptions);\n");
}

void generateGraph(char *graphName, GraphList * graph) {
	fprintf(fd, "var %s = cytoscape({ elements:	[", graphName);

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
	
	for (NodeList *aux = a; aux != NULL; aux = aux->next) {
		fprintf(fd, "{ data: { id: '%s' } },", aux->name);
	}

	if (center != NULL) {
		fprintf(fd, "{ data: { id: '%s' } },", center);
	}

	for (NodeList *aux = b; aux != NULL; aux = aux->next) {
		fprintf(fd, "{ data: { id: '%s' } },", aux->name);
	}

	switch (graph->graphType) {
		case STAR:
		case WHEEL:
			for (aux = a; aux; aux = aux->next) {
				fprintf(fd, "{ data: { id:%s-1-%s , source: '%s', target: '%s' } },", aux->name, center , aux->name, center);
			}
			if (graph->graphType == STAR)
				break;
		case CYCLE:
			for (aux = a; aux->next != NULL; aux = aux->next) {
				fprintf(fd, "{ data: { id:%s-1-%s , source: '%s', target: '%s' } },", aux->name, aux->next->name ,aux->name, aux->next->name);
			}
			fprintf(fd, "{ data: { id:%s-1-%s , source: '%s', target: '%s' } },", aux->name, a->name ,aux->name, a->name);
			break;
		case COMPLETE:
			for (aux = a; aux; aux = aux->next) {
				for (NodeList *aux2 = aux->next; aux2; aux2 = aux2->next) {
					fprintf(fd, "{ data: { source: '%s', target: '%s' } },", aux->name, aux2->name);
				}
			}
			break;
		case SIMPLE:
		default:
			break;
	}

	fprintf(fd, "]});\n");
	
}

void generateAdd(char *graphName, AddRemoveInstructionList *addList) {
	fprintf(fd, "%s.add([", graphName);

	for (AddRemoveInstructionList * aux = addList; aux != NULL; aux = aux->next) {
		if (aux->instructionType == NODE_LIST) {
			for (NodeList *node = (NodeList *)aux->addRemoveInstruction; node; node = node->next) {
				fprintf(fd, "{ data: { id: '%s' } },", node->name);
			}
		} else if (aux->instructionType == EDGE_LIST) {
			for (EdgeList *aux2 = (EdgeList *)aux->addRemoveInstruction; aux2; aux2 = aux2->next) {
				fprintf(fd, "{ data: { id: '%s-%d-%s', source: '%s', target: '%s', weight: %d } },", aux2->leftNode, aux2->weight ,aux2->rightNode, aux2->leftNode, aux2->rightNode, aux2->weight);
			}
		}
	}

	fprintf(fd, "]);\n");		
}

void generateRemove(char *graphName, AddRemoveInstructionList *addList) {
	fprintf(fd, "%s.remove(%s.$().filter([", graphName, graphName);

	for (AddRemoveInstructionList *aux = addList; aux; aux = aux->next) {
		if (aux->instructionType == NODE_LIST) {
			for (NodeList *node = (NodeList *)aux->addRemoveInstruction; node; node = node->next) {
				fprintf(fd, "'#%s',", node->name);
			}
		} else if (aux->instructionType == EDGE_LIST) {
			for (EdgeList *aux2 = (EdgeList *)aux->addRemoveInstruction; aux2; aux2 = aux2->next) {
				fprintf(fd, "'#%s-%d-%s',", aux2->leftNode, aux2->weight ,aux2->rightNode);
			}
		}
	}

	fprintf(fd, "]));\n");
}

void generateApply(char *graphName, ApplyInstructionList *applyList) {
	for (ApplyInstructionList *instruction = applyList; instruction; instruction = instruction->next) {
		char *varName = graphName;
		switch (instruction->instructionType)
		{
		case BFS_TYPE: ;
			BfsBlock *bfs = (BfsBlock *)instruction->applyInstruction;
			fprintf(fd, "var bfs = %s.elements().bfs({ roots: '%s', "
			"visit: function(v,e,u,i,depth){ if (v.data('id') === '%s') return true; }});\n"
			"bfs.path.style('background-color', 'red');\n"
			"bfs.found.style('background-color', 'red');\n", graphName, bfs->from, bfs->to);
			break;
		case DFS_TYPE: ;
			DfsBlock *dfs = (DfsBlock *)instruction->applyInstruction;
			fprintf(fd, "var dfs = %s.elements().dfs({ roots: '%s', "
			"visit: function(v, e, u, i, depth){ if (v.data('id') === '%s') return true; }});\n"
			"dfs.path.style('background-color', 'red');\n"
			"dfs.found.style('background-color', 'red');\n", graphName, bfs->from, bfs->to);
			break;
		case COLORS: ;
			ColorList *colors = (ColorList *)instruction->applyInstruction;
			for (ColorList *aux = colors; aux; aux = aux->next) {
				for (NodeList *node = aux->nodes; node; node = node->next) {
					fprintf(fd, "%s.$('#%s').style('background-color', '%s');\n", graphName, node->name, aux->rgb);
				}
			}
			break;
		case FIND_CUT_NODES:
			fprintf(fd, "var cut = %s.elements().htb().cut;\n"
			"cut.forEach(node => %s.$(node.id()).style('background-color', 'rgb(Math.floor(Math.random() * 255), Math.floor(Math.random() * 255), Math.floor(Math.random() * 255))'));\n", graphName, graphName);
			break;
		case DELETE_CUT_NODES:
			fprintf(fd, "var cutNodes = %s.elements().htb().cut;\n"
			"cutNodes.forEach( node => %s.remove(node));\n", graphName, graphName);
			break;
		case MST_TYPE:
			fprintf(fd, "var mst = %s.elements().kruskal();\n", graphName);
			varName = "mst";
			break;
		default:
			break;
		}
		fprintf(fd, "snap.start().then(() => {\n"
					"	%s.png({full: true}).then((data) => {\n"
  					"		const fs = require('fs');\n"
  					"		const out = fs.createWriteStream('%s-%d.png');\n"
					"		const stream = data.pipe(out);\n"
					"		stream.on('finish', () => {\n"
					"			console.log('Graph %s saved as %s-%d.png');\n"
					"		});\n"
					"	});\n"
					"});\n", varName, graphName, fileCounter, graphName, graphName, fileCounter);
		fileCounter++;
	}
}

