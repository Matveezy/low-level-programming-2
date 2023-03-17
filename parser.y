%code requires {
#include "../constant.h"
#include "../struct.h"
#include "../ast.h"
#include <stdio.h>
#include <string.h>
extern int yylex();
}

%{
#include <stdio.h>
void yyerror(const char *str)
{
        fprintf(stderr,"ошибка: %s\n",str);
}
#ifdef YYDEBUG
  yydebug = 1;
#endif
%}
/* Generate the parser description file. */
%verbose

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
}

%token <int_value> INT_TOKEN
%token <float_value> FLOAT_TOKEN
%token <bool_value> TOKBOOL
%token <string> TOKSTRING
%token <string> ID
%token <string> TOKFOR
%token <string> TOKIN
%token <string> QUOTE
%token <string> SEMICOLON
%token <string> COMMA
%token <string> COLON
%token <string> OBRACE
%token <string> EBRACE
%token <string> TOKFILTER
%token <string> TOKDOT
%token <string> TOKRETURN
%token <string> TOKEQUALS
%token <string> TOKPERCENT
%token <log_op> LOGOP
%token <const_op> CONSTOP


%type <string> query subqueries subquery
%type <map> map_entry map_entries map
%type <constant> value id constant
%type <condition_node> condition
%type <condition_union> condition_union conditions

%%
query: subqueries{printf("done");};

subqueries: subqueries subquery {;}
           | subquery {;}

subquery: filter {;}

filter: TOKFILTER conditions {print_condition_union($2);}

conditions:
           condition SEMICOLON{$$ = create_simple_condition_union($1);printf("conditions\n");}
           |
           condition_union
           |
           condition_union LOGOP condition {$$ = create_condition_union($1, $3, UNION, NODE, $2);}
           |
           condition LOGOP condition_union {$$ = create_condition_union($1, $3, NODE, UNION, $2);}
           |
           condition_union LOGOP condition_union {$$ = create_condition_union($1, $3, UNION, UNION, $2);}

condition_union: condition LOGOP condition {$$ = create_condition_union($1, $3, NODE, NODE, $2); printf("condition_union");}

condition:
          constant CONSTOP constant {$$ = create_condition_node($1, $3, $2); printf("condition\n");}

map:
    OBRACE map_entries EBRACE {"printf("map");"}

map_entries:
        map_entry
        |
        map_entry COMMA map_entries

map_entry:
          TOKSTRING COLON constant {printf("map_entry create");}

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