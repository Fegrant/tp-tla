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

	// No-terminales agregados
	int variable;

	// Terminales.
	token token;
	int integer;
}

// IDs y tipos de los tokens terminales generados desde Flex.
%token <token> GRAPH_TYPE
%token <token> SYMBOL

// Tipos de dato para los no-terminales generados desde Bison.
%type <program> program
%type <variable> variable


// Reglas de asociatividad y precedencia (de menor a mayor).

// El símbolo inicial de la gramatica.
%start program

%%

program: GRAPH_TYPE variable	{ $$ = ProgramGrammarAction(0); }
	;

variable: SYMBOL				{ $$ = Ack(); }
	;

%%
