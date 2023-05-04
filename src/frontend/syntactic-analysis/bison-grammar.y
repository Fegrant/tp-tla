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
	int program;
	int block;
	int instruction;
	int declaration;

	int addBlockBegin;
	int removeBlockBegin;
	int addRemoveBlock;
	int addRemoveBlockInstruction;
	int nodeList;
	int edgeList;

	int applyBlockBegin;
	int applyBlock;
	int applyBlockInstruction;
	int findCutNodes;
	int deleteCutNodes;
	int colorsBlockBegin;
	int colorsBlock;
	int colorList;
	int terminalInstuction;

	int cycleOrCompleteBlock;
	int starOrWheelBlock;
	int bipartiteCompleteBlock;
	int groupNodes;

	// Terminales.
	token token;
	int integer;
}

// IDs y tipos de los tokens terminales generados desde Flex.
%token <token> GRAPH_TYPE
%token <token> CYCLE_TYPE
%token <token> WHEEL_TYPE
%token <token> STAR_TYPE
%token <token> COMPLETE_TYPE
%token <token> BIPARTITE_COMPLETE_TYPE

// Blocks
%token <token> ADD_BLOCK
%token <token> REMOVE_BLOCK
%token <token> BEGIN_BLOCK
%token <token> APPLY_BLOCK

// Apply block actions
%token <token> BFS
%token <token> DFS
%token <token> FIND_CUT_NODES
%token <token> DELETE_CUT_NODES
%token <token> COLORS_BLOCK
%token <token> COLOR

%token <token> NODES
%token <token> EDGES
%token <token> CENTER
%token <token> GROUP
%token <token> MST

%token <token> COMMA
%token <token> GUION
%token <token> GREATER
%token <token> TO

%token <token> DIGITS
%token <token> STRING

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
%type <applyBlockBegin> applyBlockBegin
%type <applyBlock> applyBlock
%type <colorsBlockBegin> colorsBlockBegin
%type <colorList> colorList
%type <terminalInstuction> terminalInstuction

%type <cycleOrCompleteBlock> cycleOrCompleteBlock
%type <starOrWheelBlock> starOrWheelBlock
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

addBlockBegin: ADD_BLOCK STRING BEGIN_BLOCK addRemoveBlock				{ $$ = Ack(); }
	;

removeBlockBegin: REMOVE_BLOCK STRING BEGIN_BLOCK addRemoveBlock		{ $$ = Ack(); }
	;

applyBlockBegin: APPLY_BLOCK STRING BEGIN_BLOCK applyBlock				{ $$ = Ack(); }
	;

addRemoveBlock: addRemoveBlockInstruction addRemoveBlock				{ $$ = Ack(); }
	| addRemoveBlockInstruction											{ $$ = Ack(); }
	;

addRemoveBlockInstruction: NODES nodeList								{ $$ = Ack(); }
	| EDGES edgeList													{ $$ = Ack(); }
	;

nodeList: STRING COMMA nodeList											{ $$ = Ack(); }
	| STRING															{ $$ = Ack(); }
	;

edgeList: STRING GUION DIGITS GUION STRING COMMA edgeList				{ $$ = Ack(); }
	| STRING GUION STRING COMMA edgeList								{ $$ = Ack(); }
	| STRING GUION DIGITS GUION STRING									{ $$ = Ack(); }
	| STRING GUION STRING												{ $$ = Ack(); }
	;

applyBlock: applyBlockInstruction applyBlock							{ $$ = Ack(); }
	| applyBlockInstruction												{ $$ = Ack(); }
	;

applyBlockInstruction: terminalInstuction GREATER STRING				{ $$ = Ack(); }
	| terminalInstuction												{ $$ = Ack(); }
	| colorsBlockBegin GREATER STRING									{ $$ = Ack(); }
	| colorsBlockBegin													{ $$ = Ack(); }
	;

terminalInstuction: BFS STRING TO STRING								{ $$ = Ack(); }
	| DFS STRING TO STRING												{ $$ = Ack(); }
	| FIND_CUT_NODES													{ $$ = Ack(); }
	| DELETE_CUT_NODES													{ $$ = Ack(); }
	| MST																{ $$ = Ack(); }
	;

colorsBlockBegin: COLORS_BLOCK BEGIN_BLOCK colorList					{ $$ = Ack(); }
	;

colorList: COLOR nodeList colorList										{ $$ = Ack(); }
	| COLOR nodeList													{ $$ = Ack(); }
	;

declaration: GRAPH_TYPE STRING											{ $$ = Ack(); }
	| CYCLE_TYPE STRING BEGIN_BLOCK cycleOrCompleteBlock				{ $$ = Ack(); }
	| WHEEL_TYPE STRING BEGIN_BLOCK	starOrWheelBlock					{ $$ = Ack(); }
	| STAR_TYPE STRING BEGIN_BLOCK starOrWheelBlock						{ $$ = Ack(); }
	| COMPLETE_TYPE STRING BEGIN_BLOCK cycleOrCompleteBlock				{ $$ = Ack(); }
	| BIPARTITE_COMPLETE_TYPE STRING BEGIN_BLOCK bipartiteCompleteBlock	{ $$ = Ack(); }
	;

cycleOrCompleteBlock: NODES nodeList									{ $$ = Ack(); }
	;

starOrWheelBlock: CENTER STRING NODES nodeList							{ $$ = Ack(); }
	;

bipartiteCompleteBlock: groupNodes groupNodes							{ $$ = Ack(); }
	;

groupNodes: GROUP nodeList												{ $$ = Ack(); }
	;

%%
