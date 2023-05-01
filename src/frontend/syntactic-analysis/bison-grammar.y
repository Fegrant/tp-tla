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

	int addBlock;
	int removeBlock;
	int addRemoveBlock;
	int addBlockInstruction;

	int nodeList;
	int edgeList;

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
%token <token> NODES
%token <token> EDGES
%token <token> COMMA
%token <token> GUION
%token <token> INTEGER

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <block> block
%type <instruction> instruction
%type <declaration> declaration
%type <variable> variable
%type <addBlock> addBlock
%type <removeBlock> removeBlock
%type <addRemoveBlock> addRemoveBlock
%type <addBlockInstruction> addBlockInstruction
%type <nodeList> nodeList
%type <edgeList> edgeList

// Reglas de asociatividad y precedencia (de menor a mayor).

// El símbolo inicial de la gramatica.
%start program

%%

program: declaration block										{ $$ = ProgramGrammarAction(0); }
	| declaration												{ $$ = ProgramGrammarAction(0); }
	;

block: instruction block										{ $$ = Ack(); }
	| instruction												{ $$ = Ack(); }
	;

instruction: declaration										{ $$ = Ack(); }
	| addBlock													{ $$ = Ack(); }
	| removeBlock												{ $$ = Ack(); }
	;

addBlock: ADD_BLOCK SYMBOL BEGIN_BLOCK addRemoveBlock			{ $$ = Ack(); }
	;

removeBlock: REMOVE_BLOCK SYMBOL BEGIN_BLOCK addRemoveBlock		{ $$ = Ack(); }
	;

addRemoveBlock: addBlockInstruction addRemoveBlock				{ $$ = Ack(); }
	| addBlockInstruction										{ $$ = Ack(); }
	;

addBlockInstruction: NODES nodeList								{ $$ = Ack(); }
	| EDGES edgeList											{ $$ = Ack(); }
	;

nodeList: SYMBOL COMMA nodeList									{ $$ = Ack(); }
	| SYMBOL													{ $$ = Ack(); }
	;

edgeList: SYMBOL GUION INTEGER GUION SYMBOL COMMA edgeList		{ $$ = Ack(); }
	| SYMBOL GUION INTEGER GUION SYMBOL							{ $$ = Ack(); }

declaration: GRAPH_TYPE variable				{ $$ = Ack(); }
	;

variable: SYMBOL								{ $$ = Ack(); }
	;

%%
