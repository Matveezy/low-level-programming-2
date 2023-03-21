#include "lexer.h"
#include "parser.h"

int main(void) {
//    yyset_debug(1);
    char buf[1024];
    char line[1024];
    int len;
    struct query_node *root;
    while (fgets(line, 1024, stdin)) {
        strcat(buf, line);
        if (strchr(line, ';')) {
            yy_scan_string(buf);
            yyparse(root);

            yylex_destroy();
        }
    }
    return 0;
}

