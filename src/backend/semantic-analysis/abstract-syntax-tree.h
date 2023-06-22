#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

/**
* Se realiza este tipo de definiciones cuando el tipo de dato debe
* auto-referenciarse, como es el caso de una "Expression", que está compuesta
* de otras 2 expresiones.
*/
typedef struct Expression Expression;

/**
* Para cada no-terminal se define una nueva estructura que representa su tipo
* de dato y, por lo tanto, su nodo en el AST (Árbol de Sintaxis Abstracta).
*/
typedef struct {
	int value;
} Constant;

/**
* En caso de que un no-terminal ("Factor" en este caso), posea más de una
* regla de producción asociada, se crea además de la estructura un enumerado
* que identitifque cada reducción posible (cada regla de producción). Luego,
* se agrega una instancia de este enumerado dentro del nodo del no-terminal.
*
* De este modo, al recorrer el AST, es posible determinar qué nodos hijos
* posee según el valor de este enumerado.
*/
// typedef enum {
// 	EXPRESSION,
// 	CONSTANT|
// } FactorType;

// typedef struct {
// 	FactorType type;
// 	Expression * expression;
// } Factor;

// typedef enum {
// 	ADDITION,
// 	SUBTRACTION,
// 	MULTIPLICATION,
// 	DIVISION,
// 	FACTOR
// } ExpressionType;

// struct Expression {
// 	ExpressionType type;
// 	Expression * leftExpression;
// 	Expression * rightExpression;
// };

/************************** GRAPH **********************************/
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

typedef struct CycleGraph {
	struct NodeList * nodeList;
} CycleGraph;

typedef struct WheelGraph {
	char * center;
	struct NodeList * nodeList;
} WheelGraph;

typedef struct StarGraph {
	char * center;
	struct NodeList * nodeList;
} StarGraph;

typedef struct CompleteGraph {
	struct NodeList * nodeList;
} CompleteGraph;

typedef struct BipartiteCompleteGraph {
	struct NodeList * groupA;
	struct NodeList * groupB;
} BipartiteCompleteGraph;

typedef union Graph {
	struct CycleGraph cycleGraph;
	struct WheelGraph wheelGraph;
	struct StarGraph starGraph;
	struct CompleteGraph completeGraph;
	struct BipartiteCompleteGraph bipartiteCompleteGraph;
} Graph;

typedef enum GraphType {
	SIMPLE,
	CYCLE,
	WHEEL,
	STAR,
	COMPLETE,
	BIPARTITE_COMPLETE
} GraphType;

typedef struct GraphList {
	enum GraphType graphType;
	union Graph * graph;
} GraphList;


/*************************** BLOCKS *********************************/

typedef struct AddBlock {
	struct NodeList * nodes;
	struct EdgeList * edges;
} AddBlock;

typedef struct RemoveBlock {
	struct NodeList * nodes;
	struct EdgeList * edges;
} RemoveBlock;

typedef struct BfsBlock {
	char * from;
	char * to;
} BfsBlock;

typedef struct DfsBlock {
	char * from;
	char * to;
} DfsBlock;

typedef struct ColorList {
	char * rgb;
	struct NodeList * nodes;
	struct ColorList * next;
} ColorList;

typedef enum ApplyInstructionType {
	BFS_TPYE,
	DFS_TYPE,
	COLORS,
	FIND_CUT_NODES,
	DELETE_CUT_NODES,
	MST_TYPE
} ApplyInstructionType;

typedef union ApplyInstruction {
	struct BfsBlock bfs;
	struct DfsBlock dfs;
	struct ColorList colors;
} ApplyInstruction;

typedef struct ApplyInstructionList {
	enum ApplyInstructionType instructionType; 
	union ApplyInstruction * applyInstruction;
	char * outputFile;
	struct ApplyInstructionList * next;
} ApplyInstructionList;

typedef enum BlockType{
	ADD_BLOCK,
	REMOVE_BLOCK,
	APPLY_BLOCK,
	GRAPH
} BlockType;

typedef union Block {
	struct GraphList graph;
	struct AddBlock add;
	struct RemoveBlock remove;
	struct ApplyInstructionList apply;
} Block;

typedef struct BlockList {
	char * graphName;
	enum BlockType type;
	union Block * block;
	struct BlockList * next;
} BlockList;

typedef struct Program{
	// Expression * expression;
	struct BlockList * actions;
} Program;

#endif
