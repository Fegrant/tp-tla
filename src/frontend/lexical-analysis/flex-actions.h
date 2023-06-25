#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../backend/support/shared.h"
#include "../syntactic-analysis/bison-actions.h"

/**
 * Se definen las acciones a ejecutar sobre cada patrón hallado mediante el
 * analizador léxico Flex. Este analizador solo puede identificar
 * construcciones regulares, ya que utiliza un autómata finito determinístico
 * (a.k.a. DFA), como mecanismo de escaneo y reconocimiento.
 *
 * Debería existir una acción por cada patrón disponible en Flex (es decir,
 * por cada regla).
 *
 * Solo aquellas acciones que tengan interés en conocer el largo del lexema
 * emplean el parámetro adicional "length". Esto es útil cuando se desea
 * asignar memoria dinámica, y por lo tanto, se debe saber exactamente el
 * largo de aquello que se va a almacenar.
 */

// Bloque de comentarios multilínea.
void BeginCommentPatternAction();
void EndCommentPatternAction();

// Declaracion y nombre de variables
token GraphTypePatternAction(const char * lexeme);
token CycleTypePatternAction(const char * lexeme);
token WheelTypePatternAction(const char * lexeme);
token StarTypePatternAction(const char * lexeme);
token CompleteTypePatternAction(const char * lexeme);
token BipartiteCompleteTypePatternAction(const char * lexeme);

token GroupPatternAction(const char * lexeme);
token CenterPatternAction(const char * lexeme);

token StringPatternAction(const char * lexeme, const int length);
// token VariablePatternAction(const char * lexeme);
// token FileNamePatternAction(const char * lexeme);

// Bloques add y remove
token AddPatternAction(const char * lexeme);
token RemovePatternAction(const char * lexeme);
token NodesPatternAction(const char * lexeme);
token EdgesPatternAction(const char * lexeme);
token BeginBlockPatternAction(const char * lexeme);
token CommaPatternAction(const char * lexeme);
token HyphenPatternAction(const char * lexeme);
token GreaterPatternAction(const char * lexeme);

// Bloque apply
token ApplyPatternAction(const char * lexeme);
token FindPatternAction(const char * lexeme);
token DeletePatternAction(const char * lexeme);
token BfsPatternAction(const char * lexeme);
token DfsPatternAction(const char * lexeme);
token CutPatternAction(const char * lexeme);
token ToPatternAction(const char * lexeme);
token FromPatternAction(const char * lexeme);
token ColorPatternAction(const char * lexeme, const int length);
token ColorsPatternAction(const char * lexeme);
token MinimumSpanningTreeAction(const char * lexeme);

token DigitsPatternAction(const char * lexeme, const int length);

// Patrón desconocido, permite abortar debido a un error de sintaxis.
token UnknownPatternAction(const char * lexeme, const int length);

// Regla que no hace nada, permite ignorar parte de la entrada.
void IgnoredPatternAction(const char * lexeme, const int length);

#endif
