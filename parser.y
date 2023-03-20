%code requires {
#include "../constant/constant.h"
#include "../struct/struct.h"
#include "../ast/ast.h"
#include <stdio.h>
#include <string.h>
extern int yylex();
}

%{
#include <stdio.h>
void yyerror(struct query_node *root, const char *s){
    printf("error:%s\n", s);
}
#ifdef YYDEBUG
  yydebug = 1;
#endif
%}

/* Generate the parser description file. */
%verbose
%define parse.error verbose

%parse-param { struct query_node *root }

%union {
int int_value;
float float_value;
bool bool_value;
char* string;
struct map_entry *map;
enum log_operation log_op;
enum constant_operation const_op;
struct constant *constant;
struct condition_node *condition_node;
struct condition_union *condition_union;
struct return_node *return_node;
struct terminal_statement_node *term_stmt;
struct subquery_node* subquery_node;
struct for_node *for_node;
}

%token <int_value> INT_TOKEN
%token <float_value> FLOAT_TOKEN
%token <bool_value> TOKBOOL
%token <string> TOKSTRING
%token <string> ID
%token <string> TOKFOR
%token <string> TOKIN
%token <string> QUOTE
%token SEMICOLON
%token <string> COMMA
%token <string> COLON
%token <string> OBRACE
%token <string> EBRACE
%token <string> TOKFILTER
%token <string> TOKRETURN
%token <string> TOKEQUALS
%token <string> TOKPERCENT
%token <log_op> LOGOP
%token <const_op> CONSTOP
%token NEWLINE

%type <map> map_entry map_entries map
%type <constant> value id constant
%type <condition_node> condition
%type <condition_union> condition_union conditions
%type <return_node> return_value return_statement
%type <term_stmt> terminal_statement
%type <subquery_node> subquery subqueries
%type <for_node> for_statement

%%
query: for_statement SEMICOLON{root = create_query_node((void *) $1, FOR_QUERY_STATEMENT); printf("done");}

for_statement: TOKFOR ID TOKIN ID subqueries {$$ = create_for_node($2, $4, $5);}

subqueries: subqueries subquery {$$ = push_back_subquery($1, $2);}
           | subquery {$$ = $1;}

subquery:
        for_statement {$$ = create_subquery_node((void*) $1, FOR_STATEMENT); printf("subquery\n");}
        |
        terminal_statement {$$ = create_subquery_node((void*) $1, TERMINAL_STATEMENT); printf("subquery\n");}

terminal_statement: return_statement {$$ = create_terminal_statement_node((void*) $1, AST_NODE_RETURN);}

filter: TOKFILTER conditions {print_condition_union($2);}

conditions:
           condition{$$ = create_simple_condition_union($1);printf("conditions\n");}
           |
           condition_union
           |
           condition_union LOGOP condition {$$ = create_condition_union($1, $3, UNION, NODE, $2);}
           |
           condition LOGOP condition_union {$$ = create_condition_union($1, $3, NODE, UNION, $2);}
           |
           condition_union LOGOP condition_union {$$ = create_condition_union($1, $3, UNION, UNION, $2);}

condition_union: condition LOGOP condition {$$ = create_condition_union($1, $3, NODE, NODE, $2); printf("condition_union\n");}

condition:
          constant CONSTOP constant {$$ = create_condition_node($1, $3, $2); printf("condition\n");}

return_statement: TOKRETURN return_value {$$ = $2;}

return_value: constant {$$ = create_return_node($1, RETURN_CONSTANT);}
              |
              map {$$ = create_return_node($1, RETURN_MAP);}

map:
    OBRACE map_entries EBRACE {$$ = $2;}

map_entries:
        map_entry
        |
        map_entry COMMA map_entries {$$ = push_back_to_map($3, $1);}

map_entry:
          TOKSTRING COLON constant {$$ = create_map($1,$3);}

constant:
        id
        |
        value

id: ID {$$ = create_string_constant($1, true);}

value:
       INT_TOKEN {$$ = create_int_constant($1);}
       |
       FLOAT_TOKEN {$$ = create_float_constant($1);}
       |
       TOKBOOL {$$ = create_bool_constant($1);}
       |
       TOKSTRING {$$ = create_string_constant($1, false);}
%%