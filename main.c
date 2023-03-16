#include <stdio.h>
#include "lexer.h"
#include "parser.h"

int main(void) {
//    yydebug = 1;
    while (1) {

        yyparse();
    }

    return 0;
}