%{

#include "bison-actions.h"

%}

// Tipos de dato utilizados en las variables semánticas ($$, $1, $2, etc.).
%union {
	// No-terminales (backend).
	/*
	Program program;
	Expression expression;
	Factor factor;
	Constant constant;
	...
	*/

	// No-terminales (frontend).
	Program * program;
	BlockList * block;
	ApplyInstruction * instruction;
	Graph * declaration;

	int addBlockBegin;
	int removeBlockBegin;
	int addRemoveBlock;
	int addRemoveBlockInstruction;
	int nodeList;
	int edgeList;

	EdgeList edge;
	EdgeList weightedEdge;

	int applyBlockBegin;
	int applyBlock;
	int applyBlockInstruction;
	int findCutNodes;
	int deleteCutNodes;
	int colorsBlockBegin;
	int colorsBlock;
	int colorList;
	int terminalInstuction;

	int cycleBlock;
	int completeBlock;
	int starBlock;
	int wheelBlock;
	int bipartiteCompleteBlock;
	int groupNodes;

	// Terminales.
	token token;
	int integer;
	char * string;
}

// IDs y tipos de los tokens terminales generados desde Flex.
%token <token> GRAPH_TYPE
%token <token> CYCLE_TYPE
%token <token> WHEEL_TYPE
%token <token> STAR_TYPE
%token <token> COMPLETE_TYPE
%token <token> BIPARTITE_COMPLETE_TYPE

// Blocks
// add to, remove from, apply to, find cut nodes, 
// delete cut nodes, bfs from, dfs from
%token <token> ADD
%token <token> REMOVE
%token <token> BEGIN_BLOCK
%token <token> APPLY

%token <token> FROM
%token <token> CUT

// Apply block actions
%token <token> BFS
%token <token> DFS
%token <token> FIND
%token <token> DELETE
%token <token> COLORS_BLOCK
%token <token> COLOR

%token <token> NODES
%token <token> EDGES
%token <token> CENTER
%token <token> GROUP
%token <token> MST

%token <token> COMMA
%token <token> HYPHEN
%token <token> GREATER
%token <token> TO

%token <integer> DIGITS
%token <string> STRING

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <block> block
%type <instruction> instruction
%type <declaration> declaration
%type <addBlockBegin> addBlockBegin
%type <removeBlockBegin> removeBlockBegin
%type <addRemoveBlock> addRemoveBlock
%type <addRemoveBlockInstruction> addRemoveBlockInstruction
%type <applyBlockInstruction> applyBlockInstruction
%type <nodeList> nodeList
%type <edgeList> edgeList

%type <edge> edge;
%type <weightedEdge> weightedEdge;

%type <applyBlockBegin> applyBlockBegin
%type <applyBlock> applyBlock
%type <colorsBlockBegin> colorsBlockBegin
%type <colorList> colorList
%type <terminalInstuction> terminalInstuction

%type <cycleBlock> cycleBlock
%type <completeBlock> completeBlock
%type <starBlock> starBlock
%type <wheelBlock> wheelBlock
%type <bipartiteCompleteBlock> bipartiteCompleteBlock
%type <groupNodes> groupNodes


// Reglas de asociatividad y precedencia (de menor a mayor).

// El símbolo inicial de la gramatica.
%start program

%%

program: declaration block												{ $$ = ProgramGrammarAction(0); }
	| declaration														{ $$ = ProgramGrammarAction(0); }
	;

block: instruction block												{ $$ = Ack(); }
	| instruction														{ $$ = Ack(); }
	;

instruction: declaration												{ $$ = Ack(); }
	| addBlockBegin														{ $$ = Ack(); }
	| removeBlockBegin													{ $$ = Ack(); }
	| applyBlockBegin													{ $$ = Ack(); }
	;

addBlockBegin: ADD TO STRING BEGIN_BLOCK addRemoveBlock					{ $$ = CreateAddBlockGrammarAction($3, $5); }
	;

removeBlockBegin: REMOVE FROM STRING BEGIN_BLOCK addRemoveBlock			{ $$ = CreateRemoveBlockGrammarAction($3, $5); }
	;

applyBlockBegin: APPLY TO STRING BEGIN_BLOCK applyBlock					{ $$ = CreateApplyBlockGrammarAction($3, $5); }
	;

addRemoveBlock: addRemoveBlockInstruction addRemoveBlock				{ $$ = AppendBlockGrammarAction($1, $2); }
	| addRemoveBlockInstruction											{ $$ = BlockGrammarAction($1); }
	;

addRemoveBlockInstruction: NODES nodeList								{ $$ = NodeListGrammarAction($2); }
	| EDGES edgeList													{ $$ = EdgeListGrammarAction($2); }
	;

nodeList: STRING COMMA nodeList											{ $$ = AppendNodeGrammarAction($1, $3); }
	| STRING															{ $$ = CreateNodeGrammarAction($1); }
	;

edgeList: weightedEdge COMMA edgeList									{ $$ = AppendEdgeGrammarAction($1, $3); }
	| edge COMMA edgeList												{ $$ = AppendEdgeGrammarAction($1, $3); }
	| weightedEdge														{ $$ = EdgeListGrammarAction($1); }
	| edge																{ $$ = EdgeListGrammarAction($1); }
	;

weightedEdge: STRING HYPHEN DIGITS HYPHEN STRING						{ $$ = CreateWeightedEdgeGrammarAction($1, $5, $3); }
;

edge: STRING HYPHEN STRING 												{ $$ = CreateEdgeGrammarAction($1, $3); }
;

applyBlock: applyBlockInstruction applyBlock							{ $$ = Ack(); }
	| applyBlockInstruction												{ $$ = Ack(); }
	;

applyBlockInstruction: terminalInstuction GREATER STRING				{ $$ = Ack(); }
	| terminalInstuction												{ $$ = Ack(); }
	| colorsBlockBegin GREATER STRING									{ $$ = Ack(); }
	| colorsBlockBegin													{ $$ = Ack(); }
	;

terminalInstuction: BFS FROM STRING TO STRING							{ $$ = Ack(); }
	| DFS FROM STRING TO STRING											{ $$ = Ack(); }
	| FIND CUT NODES													{ $$ = Ack(); }
	| DELETE CUT NODES													{ $$ = Ack(); }
	| MST																{ $$ = Ack(); }
	;

colorsBlockBegin: COLORS_BLOCK BEGIN_BLOCK colorList					{ $$ = Ack(); }
	;

colorList: COLOR nodeList colorList										{ $$ = Ack(); }
	| COLOR nodeList													{ $$ = Ack(); }
	;

declaration: GRAPH_TYPE STRING											{ $$ = CreateSimpleGraphGrammarAction($2); }
	| CYCLE_TYPE STRING BEGIN_BLOCK cycleBlock							{ $$ = CreateCycleGraphGrammarAction($2, $4); }
	| WHEEL_TYPE STRING BEGIN_BLOCK	wheelBlock							{ $$ = CreateWheelGraphGrammarAction($2, $4); }
	| STAR_TYPE STRING BEGIN_BLOCK starBlock							{ $$ = CreateStarGraphGrammarAction($2, $4); }
	| COMPLETE_TYPE STRING BEGIN_BLOCK completeBlock					{ $$ = CreateCompleteGraphGrammarAction($2, $4); }
	| BIPARTITE_COMPLETE_TYPE STRING BEGIN_BLOCK bipartiteCompleteBlock	{ $$ = CreateBipartiteCompleteGraphGrammarAction($2, $4); }
	;

cycleBlock: NODES nodeList												{ $$ = CreateCycleBlockGrammarAction($2); }
	;

completeBlock: NODES nodeList											{ $$ = CreateCompleteBlockGrammarAction($2); }
	;

starBlock: CENTER STRING NODES nodeList									{ $$ = CreateStarBlockGrammarAction($2, $4); }
	;

wheelBlock: CENTER STRING NODES nodeList								{ $$ = CreateWheelBlockGrammarAction($2, $4); }
	;

bipartiteCompleteBlock: groupNodes groupNodes							{ $$ = CreateWheelBlockGrammarAction($2, $4); }
	;

groupNodes: GROUP nodeList												{ $$ = NodeListGrammarAction($2); }
	;

%%
