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
	BlockList * program;
	BlockList * block;
	ApplyInstruction * instruction;
	
	BlockList * declaration;
	BlockList * addBlockBegin;
	BlockList * removeBlockBegin;
	BlockList * outputGraph;
	BlockList * applyBlockBegin;
	AddRemoveInstructionList * addRemoveBlock;
	AddRemoveInstructionList * addRemoveBlockInstruction;
	NodeList * nodeList;
	EdgeList * edgeList;

	EdgeList * edge;
	EdgeList * weightedEdge;

	ApplyInstructionList * applyBlock;
	ApplyInstructionList * applyBlockInstruction;
	ApplyInstructionList * findCutNodes;
	ApplyInstructionList * deleteCutNodes;
	ApplyInstructionList * colorsBlockBegin;
	ApplyInstructionList * colorsBlock;
	ColorList * colorList;
	ApplyInstructionList * terminalInstuction;

	Graph * cycleBlock;
	Graph * completeBlock;
	Graph * starBlock;
	Graph * wheelBlock;
	Graph * bipartiteCompleteBlock;
	NodeList * groupNodes;

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
%token <string> COLOR

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
%type <outputGraph> outputGraph
%type <addRemoveBlock> addRemoveBlock
%type <addRemoveBlockInstruction> addRemoveBlockInstruction
%type <applyBlockInstruction> applyBlockInstruction
%type <nodeList> nodeList
%type <nodeList> node
%type <edgeList> edgeList

%type <edge> edge
%type <weightedEdge> weightedEdge

%type <applyBlockBegin> applyBlockBegin
%type <applyBlock> applyBlock
%type <colorsBlockBegin> colorsBlockBegin
%type <colorList> colorList
%type <colorList> colorNodes
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

program: declaration block												{ $$ = AppendProgramGrammarAction($1, $2); }
	| declaration														{ $$ = ProgramGrammarAction($1); }
	;

block: instruction block												{ $$ = AppendBlockListGrammarAction($1, $2); }
	| instruction														{ $$ = BlockListGrammarAction($1); }
	;

instruction: outputGraph												{ $$ = BlockListGrammarAction($1); }
	| declaration														{ $$ = BlockListGrammarAction($1); }
	| addBlockBegin														{ $$ = BlockListGrammarAction($1); }
	| removeBlockBegin													{ $$ = BlockListGrammarAction($1); }
	| applyBlockBegin													{ $$ = BlockListGrammarAction($1); }
	;

outputGraph: STRING GREATER STRING										{ $$ = CreateOutputGraphBlockGrammarAction($1, $3); }
	;

addBlockBegin: ADD TO STRING BEGIN_BLOCK addRemoveBlock					{ $$ = CreateAddBlockGrammarAction($3, $5); }
	;

removeBlockBegin: REMOVE FROM STRING BEGIN_BLOCK addRemoveBlock			{ $$ = CreateRemoveBlockGrammarAction($3, $5); }
	;

applyBlockBegin: APPLY TO STRING BEGIN_BLOCK applyBlock					{ $$ = CreateApplyBlockGrammarAction($3, $5); }
	;

addRemoveBlock: addRemoveBlockInstruction addRemoveBlock				{ $$ = AppendAddRemoveInstructionListGrammarAction($1, $2); }
	| addRemoveBlockInstruction											{ $$ = AddRemoveInstructionListGrammarAction($1); }
	;

addRemoveBlockInstruction: NODES nodeList								{ $$ = AddRemoveNodeListGrammarAction($2); }
	| EDGES edgeList													{ $$ = AddRemoveEdgeListGrammarAction($2); }
	;

nodeList: node COMMA nodeList											{ $$ = AppendNodeGrammarAction($1, $3); }
	| node																{ $$ = NodeListGrammarAction($1); }
	;

node: STRING															{ $$ = CreateNodeGrammarAction($1); }
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

applyBlock: applyBlockInstruction applyBlock							{ $$ = AppendApplyBlockGrammarAction($1, $2); }
	| applyBlockInstruction												{ $$ = ApplyBlockGrammarAction($1); }
	;

applyBlockInstruction: terminalInstuction GREATER STRING				{ $$ = ApplyInstructionListGrammarAction($1, $3); }
	| terminalInstuction												{ $$ = ApplyInstructionListGrammarAction($1, NULL); }
	| colorsBlockBegin GREATER STRING									{ $$ = ApplyInstructionListGrammarAction($1, $3); }
	| colorsBlockBegin													{ $$ = ApplyInstructionListGrammarAction($1, NULL); }
	;

terminalInstuction: BFS FROM STRING TO STRING							{ $$ = BfsBlockGrammarAction($3, $5); }
	| DFS FROM STRING TO STRING											{ $$ = DfsBlockGrammarAction($3, $5); }
	| FIND CUT NODES													{ $$ = CreateFindCutGrammarActions(); }
	| DELETE CUT NODES													{ $$ = CreateDeleteGrammarAction(); }
	| MST																{ $$ = CreateMstGrammarAction(); }
	;

colorsBlockBegin: COLORS_BLOCK BEGIN_BLOCK colorList					{ $$ = CreateColorsBlockGrammarAction($3); }
	;

colorList: colorNodes colorList											{ $$ = AppendColorListGrammarAction($1, $2); }
	| colorNodes														{ $$ = ColorListGrammarAction($1); }
	;

colorNodes: COLOR nodeList												{ $$ = CreateColorListGrammarAction($1, $2); }
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

starBlock: CENTER STRING NODES nodeList									{ $$ = CreateWheelBlockGrammarAction($2, $4); }
	;

wheelBlock: CENTER STRING NODES nodeList								{ $$ = CreateWheelBlockGrammarAction($2, $4); }
	;

bipartiteCompleteBlock: groupNodes groupNodes							{ $$ = CreateBipartiteCompleteBlockGrammarAction($1, $2); }
	;

groupNodes: GROUP nodeList												{ $$ = NodeListGrammarAction($2); }
	;

%%
