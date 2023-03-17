#ifndef LOW_LEVEL_PROGRAMMING_2_AST_H
#define LOW_LEVEL_PROGRAMMING_2_AST_H

#include "stdio.h"
#include "../struct/struct.h"

typedef struct ast_node ast_node;
typedef struct condition_node condition_node;
typedef struct condition_union condition_union;
typedef struct return_node return_node;

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

struct return_node {
    enum AST_NODE_TYPE node_type;
    enum return_node_type ret_node_type;
    struct constant *constant;
    struct map_entry *map;
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

struct ast_node *add_condition_node(struct ast_node *new_node, struct ast_node *parent);

condition_union *create_condition_union(void *, void *, enum condition_type,
                                        enum condition_type, enum log_operation);

condition_union *create_simple_condition_union(condition_node *cond_node);

void print_condition_union(condition_union *cond_union);

return_node *create_return_node(void *, enum return_node_type);

#endif //LOW_LEVEL_PROGRAMMING_2_AST_H
