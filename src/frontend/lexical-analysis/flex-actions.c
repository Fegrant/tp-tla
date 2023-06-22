#include "../../backend/support/logger.h"
#include "flex-actions.h"
#include <stdlib.h>
#include <string.h>

/**
 * Implementación de "flex-actions.h".
 *
 * Cada función debe realizar 2 operaciones básicas para lograr el streaming
 * de tokens hacia Bison:
 *
 *	1) Computar los atributos del token y almacenarlos donde sea conveniente
 *		(en la tabla de símbolos, en "yylval", o en ambos).
 *	2) Retornar el token que identifica el terminal identificado.
 *
 * Bison utilizará el token retornado en el punto (2) para matchear dicho
 * terminal en la gramática. Por otro lado, el valor almacenado en "yylval" es
 * el que Bison proveerá como valor semántico al realizar una reducción
 * (mediante $1, $2, $3, etc.).
 */

void BeginCommentPatternAction() {
	LogDebug("BeginCommentPatternAction.");
}

void EndCommentPatternAction() {
	LogDebug("EndCommentPatternAction.");
}

token GraphTypePatternAction(const char * lexeme) {
	LogDebug("GraphTypePatternAction: '%s'.", lexeme);
	yylval.token = GRAPH_TYPE;
	return GRAPH_TYPE;
}

token CycleTypePatternAction(const char * lexeme) {
	LogDebug("CycleTypePatternAction: '%s'.", lexeme);
	yylval.token = CYCLE_TYPE;
	return CYCLE_TYPE;
}

token WheelTypePatternAction(const char * lexeme) {
	LogDebug("WheelTypePatternAction: '%s'.", lexeme);
	yylval.token = WHEEL_TYPE;
	return WHEEL_TYPE;
}

token StarTypePatternAction(const char * lexeme) {
	LogDebug("StarTypePatternAction: '%s'.", lexeme);
	yylval.token = STAR_TYPE;
	return STAR_TYPE;
}

token CompleteTypePatternAction(const char * lexeme) {
	LogDebug("GraphTypePatternAction: '%s'.", lexeme);
	yylval.token = COMPLETE_TYPE;
	return COMPLETE_TYPE;
}

token BipartiteCompleteTypePatternAction(const char * lexeme) {
	LogDebug("BipartiteCompleteTypePatternAction: '%s'.", lexeme);
	yylval.token = BIPARTITE_COMPLETE_TYPE;
	return BIPARTITE_COMPLETE_TYPE;
}

token GroupPatternAction(const char * lexeme) {
	LogDebug("GroupPatternAction: '%s'.", lexeme);
	yylval.token = GROUP;
	return GROUP;
}

token CenterPatternAction(const char * lexeme) {
	LogDebug("CenterPatternAction: '%s'.", lexeme);
	yylval.token = CENTER;
	return CENTER;
}

token AddPatternAction(const char * lexeme) {
	LogDebug("AddPatternAction: '%s'.", lexeme);
	yylval.token = ADD;
	return ADD;
}

token RemovePatternAction(const char * lexeme) {
	LogDebug("RemovePatternAction: '%s'.", lexeme);
	yylval.token = REMOVE;
	return REMOVE;
}

token ApplyPatternAction(const char * lexeme) {
	LogDebug("ApplyPatternAction: '%s'.", lexeme);
	yylval.token = APPLY;
	return APPLY;
}

token FindPatternAction(const char * lexeme) {
	LogDebug("FindPatternAction: '%s'.", lexeme);
	yylval.token = FIND;
	return FIND;
}

token DeletePatternAction(const char * lexeme) {
	LogDebug("DeletePatternAction: '%s'.", lexeme);
	yylval.token = DELETE;
	return DELETE;
}

token BfsPatternAction(const char * lexeme) {
	LogDebug("BfsPatternAction: '%s'.", lexeme);
	yylval.token = BFS;
	return BFS;
}

token DfsPatternAction(const char * lexeme) {
	LogDebug("DfsPatternAction: '%s'.", lexeme);
	yylval.token = DFS;
	return DFS;
}

token CutPatternAction(const char * lexeme) {
	LogDebug("CutPatternAction: '%s'.", lexeme);
	yylval.token = CUT;
	return CUT;
}

token ToPatternAction(const char * lexeme) {
	LogDebug("ToPatternAction: '%s'.", lexeme);
	yylval.token = TO;
	return TO;
}

token FromPatternAction(const char * lexeme) {
	LogDebug("FromPatternAction: '%s'.", lexeme);
	yylval.token = FROM;
	return FROM;
}

token ColorPatternAction(const char * lexeme, const int length) {
	LogDebug("NodeColorPatternAction: '%s'.", lexeme);
	char *text = calloc(length + 1, sizeof(char));
	strncpy(text, lexeme, length);
	yylval.token = COLOR;
	yylval.string = text;
	return COLOR;
}

token ColorsPatternAction(const char * lexeme) {
	LogDebug("ColorsPatternAction: '%s'.", lexeme);
	yylval.token = COLORS_BLOCK;
	return COLORS_BLOCK;
}

token NodesPatternAction(const char * lexeme) {
	LogDebug("NodesPatternAction: '%s'.", lexeme);
	yylval.token = NODES;
	return NODES;
}

token EdgesPatternAction(const char * lexeme) {
	LogDebug("EdgesPatternAction: '%s'.", lexeme);
	yylval.token = EDGES;
	return EDGES;
}

token MinimumSpanningTreeAction(const char * lexeme) {
	LogDebug("EdgesPatternAction: '%s'.", lexeme);
	yylval.token = MST;
	return MST;
}

token BeginBlockPatternAction(const char * lexeme) {
	LogDebug("BeginBlockPatternAction: '%s'.", lexeme);
	yylval.token = BEGIN_BLOCK;
	return BEGIN_BLOCK;
}

token CommaPatternAction(const char * lexeme) {
	LogDebug("CommaPatternAction: '%s'.", lexeme);
	yylval.token = COMMA;
	return COMMA;
}

token HyphenPatternAction(const char * lexeme) {
	LogDebug("HyphenPatternAction: '%s'.", lexeme);
	yylval.token = HYPHEN;
	return HYPHEN;
}

token GreaterPatternAction(const char * lexeme) {
	LogDebug("GreaterPatternAction: '%s'.", lexeme);
	yylval.token = GREATER;
	return GREATER;
}

token StringPatternAction(const char * lexeme, const int length) {
	LogDebug("StringPatternAction: '%s'.[len=%d]", lexeme, length);
	char *text = calloc(length+1, sizeof(char));
	strncpy(text, lexeme, length);
	yylval.string = text;
	return STRING;
}

// token VariablePatternAction(const char * lexeme) {
// 	LogDebug("VariablePatternAction: '%s'.", lexeme);
// 	yylval.token = VARIABLE;
// 	return VARIABLE;
// }

// token FileNamePatternAction(const char * lexeme) {
// 	LogDebug("FileNamePatternAction: '%s'.", lexeme);
// 	yylval.token = FILENAME;
// 	return FILENAME;
// }

token DigitsPatternAction(const char * lexeme, const int length) {
	LogDebug("DigitsPatternAction: '%s' (length = %d).", lexeme, length);
	yylval.integer = atoi(lexeme);
	return DIGITS;
}

token UnknownPatternAction(const char * lexeme, const int length) {
	LogDebug("UnknownPatternAction: '%s' (length = %d).", lexeme, length);
	yylval.token = YYUNDEF;
	// Al emitir este token, el compilador aborta la ejecución.
	return YYUNDEF;
}

void IgnoredPatternAction(const char * lexeme, const int length) {
	LogDebug("IgnoredPatternAction: '%s' (length = %d).", lexeme, length);
	// Como no debe hacer nada con el patrón, solo se loguea en consola.
}
