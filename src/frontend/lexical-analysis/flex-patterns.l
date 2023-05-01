%{

#include "flex-actions.h"

%}

/* Obliga a crear una regla explícita para manejar lexemas desconocidos. */
%option nodefault

/* Escanea un único archivo y se detiene. */
%option noyywrap

/* Dispone una variable global "yylineno" con el número de línea actual. */
%option yylineno

/* Contextos de Flex. */
%x COMMENT

/* Patrones reutilizables. */
digit           [0-9]
endline			[\n]
whitespace		[ \f\n\r\t\v]
letter          [a-zA-Z]

%%

"/*"										{ BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>.									{ ; }
<COMMENT>{endline}							{ ; }
<COMMENT>"*/"								{ EndCommentPatternAction(); BEGIN(INITIAL); }

"Graph"                                     { return GraphTypePatternAction(yytext); }
"add to"                                    { return AddBlockPatternAction(yytext); }
"remove from"                               { return RemoveBlockPatternAction(yytext); }
"nodes "                                    { return NodesPatternAction(yytext); }
"edges "                                    { return EdgesPatternAction(yytext); }
":"                                         { return BeginBlockPatternAction(yytext); }
","                                         { return CommaPatternAction(yytext); }
"-"                                         { return GuionPatternAction(yytext); }

{digit}+                                    { return IntegerPatternAction(yytext, yyleng); }
{whitespace}								{ IgnoredPatternAction(yytext, yyleng); }
{letter}+                                   { return SymbolPatternAction(yytext); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%
