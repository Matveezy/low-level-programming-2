#ifndef LOW_LEVEL_PROGRAMMING_2_AST_H
#define LOW_LEVEL_PROGRAMMING_2_AST_H
#include "struct.h"
typedef struct ast_node ast_node;

struct ast_node {
    enum AST_NODE_TYPE node_type;
    union {
        struct {
            enum AST_NODE_TYPE node_type;
            char *attrname;
            enum constant_operation constant_op;
            struct constant *constant;
            struct ast_node *ast_condition_node;
        } condition_node;
    };
};
#endif //LOW_LEVEL_PROGRAMMING_2_AST_H
