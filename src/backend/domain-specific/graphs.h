#ifndef GRAPHS_H
#define GRAPHS_H

#define TRUE 1
#define FALSE 0

typedef struct NodeList {
	char * name;
	struct NodeList * next;
} NodeList;

typedef struct EdgeList {
	char * leftNode;
	char * rightNode;
	int weight;
	struct EdgeList * next;
} EdgeList;

NodeList *createNode(char *name);
int nodeExistsRecursive(NodeList * nodes, char *name);
NodeList *addNodeRecursive(NodeList *nodes, char *name, int *repeated);
NodeList *removeNodeRecursive(NodeList *nodes, char *name, int *wasFound);
EdgeList *createEdge(char *leftNodeName, char* rightNodeName, int weight);
EdgeList *addEdgeRecursive(EdgeList * edges, char * leftNodeName, char * rightNodeName, int weight, int *repeated);
EdgeList *removeEdgeRecursive(EdgeList * edges, char * leftNodeName, char * rightNodeName, int weight, int *wasFound);
NodeList *removeNode(NodeList *nodes, char *name);

#endif