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
	int variable;

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

	// Terminales.
	token token;
	int integer;
}

// IDs y tipos de los tokens terminales generados desde Flex.
%token <token> GRAPH_TYPE
%token <token> SYMBOL
%token <token> ADD_BLOCK
%token <token> REMOVE_BLOCK
%token <token> BEGIN_BLOCK
%token <token> COLORS_BLOCK
%token <token> NODES
%token <token> EDGES
%token <token> COMMA
%token <token> GUION
%token <token> INTEGER
%token <token> APPLY_BLOCK
%token <token> BFS
%token <token> DFS
%token <token> TO
%token <token> FIND_CUT_NODES
%token <token> DELETE_CUT_NODES
%token <token> COLOR

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <block> block
%type <instruction> instruction
%type <declaration> declaration
%type <variable> variable
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

addBlockBegin: ADD_BLOCK SYMBOL BEGIN_BLOCK addRemoveBlock				{ $$ = Ack(); }
	;

removeBlockBegin: REMOVE_BLOCK SYMBOL BEGIN_BLOCK addRemoveBlock		{ $$ = Ack(); }
	;

applyBlockBegin: APPLY_BLOCK SYMBOL BEGIN_BLOCK applyBlock				{ $$ = Ack(); }
	;

addRemoveBlock: addRemoveBlockInstruction addRemoveBlock				{ $$ = Ack(); }
	| addRemoveBlockInstruction											{ $$ = Ack(); }
	;

addRemoveBlockInstruction: NODES nodeList								{ $$ = Ack(); }
	| EDGES edgeList													{ $$ = Ack(); }
	;

nodeList: SYMBOL COMMA nodeList											{ $$ = Ack(); }
	| SYMBOL															{ $$ = Ack(); }
	;

edgeList: SYMBOL GUION INTEGER GUION SYMBOL COMMA edgeList				{ $$ = Ack(); }
	| SYMBOL GUION SYMBOL COMMA edgeList								{ $$ = Ack(); }
	| SYMBOL GUION INTEGER GUION SYMBOL									{ $$ = Ack(); }
	| SYMBOL GUION SYMBOL												{ $$ = Ack(); }
	;

applyBlock: applyBlockInstruction applyBlock							{ $$ = Ack(); }
	| applyBlockInstruction												{ $$ = Ack(); }
	;

applyBlockInstruction: BFS SYMBOL TO SYMBOL								{ $$ = Ack(); }
	| DFS SYMBOL TO SYMBOL												{ $$ = Ack(); }
	| colorsBlockBegin													{ $$ = Ack(); }
	| FIND_CUT_NODES													{ $$ = Ack(); }
	| DELETE_CUT_NODES													{ $$ = Ack(); }
	;

colorsBlockBegin: COLORS_BLOCK BEGIN_BLOCK colorList					{ $$ = Ack(); }
	;

colorList: COLOR nodeList colorList										{ $$ = Ack(); }
	| COLOR nodeList													{ $$ = Ack(); }
	;

declaration: GRAPH_TYPE variable										{ $$ = Ack(); }
	;

variable: SYMBOL														{ $$ = Ack(); }
	;

%%
