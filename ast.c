#include "ast.h"

char *union_type_to_string(enum condition_union_type cond_union_type);

char *const_type_to_string(enum CONSTANT_TYPE const_type);

char *const_op_type_to_string(enum constant_operation cons_op);

void *print_const_value(struct constant *constant);

void print_const(struct constant *left_const);

void print_simple_condition(condition_union *simple_cond_union);

condition_node *
create_condition_node(struct constant *left_value, struct constant *right_value, enum constant_operation const_op) {
    condition_node *condition_node = malloc(sizeof(struct condition_node));
    condition_node->node_type = AST_NODE_CONDITION;
    condition_node->left_value = left_value;
    condition_node->right_value = right_value;
    condition_node->constant_op = const_op;
    return condition_node;
}

struct ast_node *add_condition_node(struct ast_node *new_node, struct ast_node *parent) {
    parent->condition_node.next_condition_node = new_node;
    return new_node;
}

condition_union *create_condition_union(void *left_value, void *right_value, enum condition_type left_type,
                                        enum condition_type right_type, enum log_operation log_op) {
    condition_union *cond_union = malloc(sizeof(struct condition_union));
    switch (left_type) {
        case NODE:
            cond_union->left_side.left_condition_node = (condition_node *) left_value;
            break;
        case UNION:
            cond_union->left_side.left_union = (condition_union *) left_value;
            break;
    }
    switch (right_type) {
        case NODE:
            cond_union->right_side.right_condition_node = (condition_node *) right_value;
            break;
        case UNION:
            cond_union->right_side.right_union = (condition_union *) right_value;
            break;
    }
    cond_union->union_type = UNION_CONDITION;
    cond_union->left_condition_type = left_type;
    cond_union->right_condition_type = right_type;
    cond_union->log_operation = log_op;
    return cond_union;
}

condition_union *create_simple_condition_union(condition_node *cond_node) {
    condition_union *simple_cond_union = malloc(sizeof(struct condition_union));
    simple_cond_union->union_type = SIMPLE_CONDITION;
    simple_cond_union->left_side.left_condition_node = cond_node;
    simple_cond_union->left_condition_type = NODE;
    return simple_cond_union;
}

void print_condition_union(condition_union *cond_union) {
    if (cond_union->union_type == SIMPLE_CONDITION) {
        print_simple_condition(cond_union);
    }
}

void print_simple_condition(condition_union *simple_cond_union) {
    printf("condition_type: %s\n", union_type_to_string(simple_cond_union->union_type));
    printf("\toperation: %s\n", const_op_type_to_string(simple_cond_union->left_side.left_condition_node->constant_op));
    printf("\tleft_value:\n");
    print_const(simple_cond_union->left_side.left_condition_node->left_value);
    printf("\tright_value:\n");
    print_const(simple_cond_union->left_side.left_condition_node->right_value);
}

void print_const(struct constant *left_const) {
    left_const->is_string_ref == 1 ? printf("\t\ttype: reference\n") : printf("\t\ttype: constant\n");
    printf("\t\tconstant_type:%s\n", const_type_to_string(left_const->const_type));
    printf("\t\tvalue:");
    print_const_value(left_const);
    printf("\n");
}

char *union_type_to_string(enum condition_union_type cond_union_type) {
    if (cond_union_type == SIMPLE_CONDITION) {
        return "simple_condition";
    }
    return "condition_union";
}

char *const_op_type_to_string(enum constant_operation cons_op) {
    switch (cons_op) {
        case GTE:
            return ">=";
        case LTE:
            return "<=";
        case EQUALS:
            return "==";
        case NEQUALS:
            return "!=";
        case GT:
            return ">";
        case LT:
            return "<";
        case LIKE:
            return "LIKE";
    }
}

char *const_type_to_string(enum CONSTANT_TYPE const_type) {
    switch (const_type) {
        case FLOAT:
            return "float";
        case INT:
            return "int";
        case STRING:
            return "string";
        case BOOL:
            return "bool";
    }
}

void *print_const_value(struct constant *constant) {
    switch (constant->const_type) {
        case FLOAT:
            printf("%f", constant->float_data);
            break;
        case INT:
            printf("%d", constant->int_data);
            break;
        case BOOL:
            printf("%s", constant->bool_data ? "true" : "false");
            break;
        case STRING:
            printf("%s", constant->string);
            break;
    }
}

