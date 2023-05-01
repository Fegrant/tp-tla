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

token SymbolPatternAction(const char * lexeme) {
	LogDebug("SymbolPatternAction: '%s'.", lexeme);
	yylval.token = SYMBOL;
	return SYMBOL;
}

token IntegerPatternAction(const char * lexeme, const int length) {
	LogDebug("IntegerPatternAction: '%s' (length = %d).", lexeme, length);
	yylval.integer = atoi(lexeme);
	return INTEGER;
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
