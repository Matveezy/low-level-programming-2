%code requires {
#include "../constant.h"
#include "../struct.h"
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
%token <string> OBRACE
%token <string> EBRACE
%token <string> TOKFILTER
%token <string> TOKDOT
%token <string> TOKRETURN
%token <string> TOKEQUALS
%token <string> TOKPERCENT
%token <log_op> LOGOP
%token <const_op> CONSTOP


%type <string> query subqueries subquery filters condition pattern
%type <map> map_entry
%type <const> value

%%
query: subqueries {printf("%s", $1);};


subqueries: subqueries subquery {;}
           | subquery {;}

subquery: filters {;}

filters: {;}
    | TOKFILTER condition {printf("filters1\n");}
    | filters LOGOP conditions {printf("filters2\n");}

conditions: {;}
    | condition {;}

condition:
       map_entry {printf("map_entry is: %s\n%s\n%s\n", $1->alias, $1->attrname, $1->char_predicate);};


map_entry:
     LOWERCASE_WORD TOKDOT LOWERCASE_WORD TOKEQUALS QUOTE LOWERCASE_WORD QUOTE {$$=create_map_entry($1,$3,$6);}
     |
     LOWERCASE_WORD TOKDOT LOWERCASE_WORD TOKEQUALS TOKBOOL {$$=create_map_entry($1, $3, (bool) $5);}
     |
     LOWERCASE_WORD TOKDOT LOWERCASE_WORD CONSTOP pattern {$$=create_map_entry($1,$3,$5);}

const_operation:
                LOWERCASE_WORD TOKDOT LOWERCASE_WORD LOGOP value {printf("const_operation founded");}

pattern:
     QUOTE LOWERCASE_WORD QUOTE {$$ = $2;}
     |
     QUOTE TOKPERCENT LOWERCASE_WORD QUOTE {$$ = $2;}
     |
     QUOTE TOKPERCENT LOWERCASE_WORD TOKPERCENT {$$ = $2;}
     |
     QUOTE LOWERCASE_WORD TOKPERCENT {$$ = $2;}

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