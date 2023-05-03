#include "../../backend/support/logger.h"
#include "flex-actions.h"
#include <stdlib.h>

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

token AddBlockPatternAction(const char * lexeme) {
	LogDebug("AddBlockPatternAction: '%s'.", lexeme);
	yylval.token = ADD_BLOCK;
	return ADD_BLOCK;
}

token RemoveBlockPatternAction(const char * lexeme) {
	LogDebug("RemoveBlockPatternAction: '%s'.", lexeme);
	yylval.token = REMOVE_BLOCK;
	return REMOVE_BLOCK;
}

token ApplyBlockPatternAction(const char * lexeme) {
	LogDebug("ApplyBlockPatternAction: '%s'.", lexeme);
	yylval.token = APPLY_BLOCK;
	return APPLY_BLOCK;
}

token FindCutNodesPatternAction(const char * lexeme) {
	LogDebug("FindCutNodesPatternAction: '%s'.", lexeme);
	yylval.token = FIND_CUT_NODES;
	return FIND_CUT_NODES;
}

token DeleteCutNodesPatternAction(const char * lexeme) {
	LogDebug("DeleteCutNodesPatternAction: '%s'.", lexeme);
	yylval.token = DELETE_CUT_NODES;
	return DELETE_CUT_NODES;
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

token ToPatternAction(const char * lexeme) {
	LogDebug("ToPatternAction: '%s'.", lexeme);
	yylval.token = TO;
	return TO;
}

token ColorPatternAction(const char * lexeme) {
	LogDebug("NodeColorPatternAction: '%s'.", lexeme);
	yylval.token = COLOR;
	return COLOR;
}

token ColorsBlockPatternAction(const char * lexeme) {
	LogDebug("ColorsBlockPatternAction: '%s'.", lexeme);
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

token GuionPatternAction(const char * lexeme) {
	LogDebug("GuionPatternAction: '%s'.", lexeme);
	yylval.token = GUION;
	return GUION;
}

token GreaterPatternAction(const char * lexeme) {
	LogDebug("GreaterPatternAction: '%s'.", lexeme);
	yylval.token = GREATER;
	return GREATER;
}

token StringPatternAction(const char * lexeme) {
	LogDebug("StringPatternAction: '%s'.", lexeme);
	yylval.token = STRING;
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
