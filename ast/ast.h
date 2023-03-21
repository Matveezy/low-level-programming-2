#ifndef LOW_LEVEL_PROGRAMMING_2_AST_H
#define LOW_LEVEL_PROGRAMMING_2_AST_H

#include "stdio.h"
#include "../struct/struct.h"

typedef struct ast_node ast_node;
typedef struct condition_node condition_node;
typedef struct condition_union condition_union;
typedef struct return_node return_node;
typedef struct map_entry map_entry;
typedef struct terminal_statement_node terminal_statement_node;
typedef struct subquery_node subquery_node;
typedef struct query_node query_node;
typedef struct for_node for_node;
typedef struct insert_node insert_node;
typedef struct remove_node remove_node;
typedef struct update_node update_node;
typedef struct create_table_node create_node;
typedef struct drop_table_node drop_table_node;
typedef struct filter_node filter_node;

enum condition_type {
    NODE,
    UNION
};

enum condition_union_type {
    UNION_CONDITION,
    SIMPLE_CONDITION
};

enum return_node_type {
    RETURN_CONSTANT,
    RETURN_MAP
};

enum subquery_node_type {
    FOR_STATEMENT,
    FILTER_STATEMENT,
    TERMINAL_STATEMENT
};

enum query_node_type {
    FOR_QUERY_STATEMENT,
    INSERT_QUERY_STATEMENT,
    CREATE_QUERY_STATEMENT,
    DROP_QUERY_STATEMENT
};

struct return_node {
    enum AST_NODE_TYPE node_type;
    enum return_node_type ret_node_type;
    struct constant *constant;
    map_entry *map;
};

struct for_node {
    char *alias;
    char *table_name;
    subquery_node *subquery_list;
};

struct remove_node {
    char *table_name;
    char *variable;
};

struct update_node {
    char *table_name;
    char *variable;
    map_entry *map;
};

struct create_table_node {
    char *table_name;
    map_entry *columns;
};

struct drop_table_node {
    char *table_name;
};

struct filter_node {
    condition_union *condition;
};

struct condition_node {
    enum AST_NODE_TYPE node_type;
    enum constant_operation constant_op;
    struct constant *left_value;
    struct constant *right_value;
};

struct condition_union {
    union {
        struct condition_union *right_union;
        struct condition_node *right_condition_node;
    } right_side;
    union {
        struct condition_union *left_union;
        struct condition_node *left_condition_node;
    } left_side;
    enum condition_union_type union_type;
    enum condition_type left_condition_type;
    enum condition_type right_condition_type;
    enum log_operation log_operation;
};

struct query_node {
    enum query_node_type query_node_type;
    void *statement;
};

struct subquery_node {
    enum subquery_node_type subquery_type;
    void *statement;
    struct subquery_node *next;
    struct subquery_node *prev;
};

struct terminal_statement_node {
    enum AST_NODE_TYPE terminal_stmt_type;
    void *terminal_statement;
};

struct insert_node {
    char *tablename;
    map_entry *map;
};

struct ast_node {
    enum AST_NODE_TYPE node_type;
    union {
        struct {
            enum AST_NODE_TYPE node_type;
            char *attrname;
            enum constant_operation constant_op;
            struct constant *constant;
            struct ast_node *next_condition_node;
        } condition_node;
    };
};

condition_node *create_condition_node(struct constant *, struct constant *, enum constant_operation);

struct ast_node *add_condition_node(struct ast_node *, struct ast_node *);

condition_union *create_condition_union(void *, void *, enum condition_type,
                                        enum condition_type, enum log_operation);

condition_union *create_simple_condition_union(condition_node *);

void print_condition_union(condition_union *, int);

return_node *create_return_node(void *, enum return_node_type);

terminal_statement_node *create_terminal_statement_node(void *, enum AST_NODE_TYPE);

subquery_node *create_subquery_node(void *, enum subquery_node_type);

subquery_node *push_back_subquery(subquery_node *, subquery_node *);

query_node *create_query_node(void *, enum query_node_type);

for_node *create_for_node(char *, char *, subquery_node *);

insert_node *create_insert_node(char *, map_entry *);

remove_node *create_remove_node(char *, char *);

update_node *create_update_node(char *, char *, map_entry *);

create_node *init_create_table_node(char *, map_entry *);

drop_table_node *create_drop_table_node(char *);

filter_node *create_filter_node(condition_union *);

void print_ast(query_node *, int);
#endif //LOW_LEVEL_PROGRAMMING_2_AST_H
