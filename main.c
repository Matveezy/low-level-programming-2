#include "lexer.h"
#include "parser.h"

int main(void) {
    yyset_debug(1);
    while (1) {

        yyparse();
    }

    return 0;
}