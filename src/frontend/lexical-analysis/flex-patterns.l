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
color           #[0-9a-fA-F]{6}
digit           [0-9]
endline			[\n]
whitespace		[ \f\n\r\t\v]
letter          [a-zA-Z]

%%

"/*"										{ BEGIN(COMMENT); BeginCommentPatternAction(); }
<COMMENT>.									{ ; }
<COMMENT>{endline}							{ ; }
<COMMENT>"*/"								{ EndCommentPatternAction(); BEGIN(INITIAL); }

"Graph "                                    { return GraphTypePatternAction(yytext); }
"Cycle "                                    { return CycleTypePatternAction(yytext); }
"Wheel "                                    { return WheelTypePatternAction(yytext); }
"Star "                                     { return StarTypePatternAction(yytext); }
"Complete "                                 { return CompleteTypePatternAction(yytext); }
"BipartiteComplete "                        { return BipartiteCompleteTypePatternAction(yytext); }

"group "                                    { return GroupPatternAction(yytext); }
"center "                                   { return CenterPatternAction(yytext); }

"add to "                                   { return AddBlockPatternAction(yytext); }
"remove from "                              { return RemoveBlockPatternAction(yytext); }
"apply to "                                 { return ApplyBlockPatternAction(yytext); }
"find cut nodes"                            { return FindCutNodesPatternAction(yytext); }
"delete cut nodes"                          { return DeleteCutNodesPatternAction(yytext); }
"bfs from "                                 { return BfsPatternAction(yytext); }
"dfs from "                                 { return DfsPatternAction(yytext); }
"colors"                                    { return ColorsBlockPatternAction(yytext); }
"nodes "                                    { return NodesPatternAction(yytext); }
"edges "                                    { return EdgesPatternAction(yytext); }
"mst"                                       { return MinimumSpanningTreeAction(yytext); }

"to"                                        { return ToPatternAction(yytext); }
":"                                         { return BeginBlockPatternAction(yytext); }
","                                         { return CommaPatternAction(yytext); }
"-"                                         { return GuionPatternAction(yytext); }

">"                                         { return GreaterPatternAction(yytext); }

{color}                                     { return ColorPatternAction(yytext); }
{digit}+                                    { return DigitsPatternAction(yytext, yyleng); }
{whitespace}								{ IgnoredPatternAction(yytext, yyleng); }
{letter}+                                   { return StringPatternAction(yytext); }

.											{ return UnknownPatternAction(yytext, yyleng); }

%%
