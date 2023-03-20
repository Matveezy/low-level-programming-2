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
struct insert_node *insert_node;
struct remove_node *remove_node;
struct update_node *update_node;
struct create_table_node *create_table_node;
struct drop_table_node *drop_table_node;
struct filter_node *filter_node;
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
%token TOKWITH
%token TOKINSERT
%token TOKINTO
%token TOKREMOVE
%token TOKUPDATE
%token TOKCREATE
%token TOKDROP
%token TOKTABLE


%type <map> map_entry map_entries map
%type <constant> value id constant
%type <condition_node> condition
%type <condition_union> condition_union conditions
%type <return_node> return_value return_statement
%type <filter_node> filter
%type <term_stmt> terminal_statement
%type <subquery_node> subquery subqueries
%type <for_node> for_statement
%type <insert_node> insert_statement
%type <remove_node> remove_statement
%type <update_node> update_statement
%type <create_table_node> create_table_statement
%type <drop_table_node> drop_statement

%%
query: for_statement SEMICOLON {root = create_query_node((void *) $1, FOR_QUERY_STATEMENT); printf("done\n");}
       |
       insert_statement SEMICOLON {root = create_query_node((void *) $1, INSERT_QUERY_STATEMENT); printf("done\n");}
       |
       create_table_statement SEMICOLON {root = create_query_node((void *) $1, CREATE_QUERY_STATEMENT); printf("done\n");}
       |
       drop_statement SEMICOLON {root = create_query_node((void *) $1, DROP_QUERY_STATEMENT); printf("done\n");}

for_statement: TOKFOR ID TOKIN ID subqueries {$$ = create_for_node($2, $4, $5);}

subqueries: subqueries subquery {$$ = push_back_subquery($1, $2);}
           | subquery {$$ = $1;}

subquery:
        for_statement {$$ = create_subquery_node((void*) $1, FOR_STATEMENT); printf("subquery\n");}
        |
        terminal_statement {$$ = create_subquery_node((void*) $1, TERMINAL_STATEMENT); printf("subquery\n");}
        |
        filter {$$ = create_subquery_node((void*) $1, FILTER_STATEMENT); printf("subquery\n");}

terminal_statement: return_statement {$$ = create_terminal_statement_node((void*) $1, AST_NODE_RETURN);}
                    |
                    remove_statement {$$ = create_terminal_statement_node((void*) $1, AST_NODE_REMOVE);}
                    |
                    update_statement {$$ = create_terminal_statement_node((void*) $1, AST_NODE_UPDATE);}

filter: TOKFILTER conditions {$$ = create_filter_node($2);}

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

update_statement : TOKUPDATE id TOKWITH map TOKIN id {$$ = create_update_node($6->string, $2->string, $4);}

remove_statement: TOKREMOVE id TOKIN id {$$ = create_remove_node($4->string, $2->string);}

return_statement: TOKRETURN return_value {$$ = $2;}

return_value: constant {$$ = create_return_node($1, RETURN_CONSTANT);}
              |
              map {$$ = create_return_node($1, RETURN_MAP);}

insert_statement : TOKINSERT map TOKINTO id {$$ = create_insert_node($4->string, $2);}

map:
    OBRACE map_entries EBRACE {$$ = $2;}

map_entries:
        map_entry
        |
        map_entry COMMA map_entries {$$ = push_back_to_map($1, $3);}

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

create_table_statement : TOKCREATE TOKTABLE id map {$$ = init_create_table_node($3->string, $4);};

drop_statement : TOKDROP TOKTABLE id {$$ = create_drop_table_node($3->string);}
%%