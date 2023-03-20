#include "ast.h"

char *union_type_to_string(enum condition_union_type cond_union_type);

char *const_type_to_string(enum CONSTANT_TYPE const_type);

char *const_op_type_to_string(enum constant_operation cons_op);

void *print_const_value(struct constant *constant);

void print_const(struct constant *left_const);

void print_simple_condition(condition_union *simple_cond_union);

map_entry *get_first_map_entry(map_entry *last_entry);

subquery_node *get_first_subquery(subquery_node *last_subquery);

query_node *create_query_node(void *statement, enum query_node_type query_node_type) {
    query_node *query_node = malloc(sizeof(struct query_node));
    query_node->query_node_type = query_node_type;
    query_node->statement = statement;
    return query_node;
}

for_node *create_for_node(char *alias, char *table_name, subquery_node *subquery_list) {
    struct for_node *created_for_node = malloc(sizeof(struct for_node));
    created_for_node->alias = alias;
    created_for_node->table_name = table_name;
    created_for_node->subquery_list = get_first_subquery(subquery_list);
    return created_for_node;
}

insert_node *create_insert_node(char *table_name, map_entry *map) {
    insert_node *ins_node = malloc(sizeof(struct insert_node));
    ins_node->tablename = table_name;
    ins_node->map = get_first_map_entry(map);
    return ins_node;
}

update_node *create_update_node(char *table_name, char *variable, map_entry *map) {
    update_node *upd_node = malloc(sizeof(struct update_node));
    upd_node->table_name = table_name;
    upd_node->variable = variable;
    upd_node->map = get_first_map_entry(map);;
    return upd_node;
}

remove_node *create_remove_node(char *table_name, char *variable) {
    remove_node *rem_node = malloc(sizeof(struct remove_node));
    rem_node->table_name = table_name;
    rem_node->variable = variable;
    return rem_node;
}

create_node *init_create_table_node(char *table_name, map_entry *columns) {
    create_node *crt_table = malloc(sizeof(struct create_table_node));
    crt_table->table_name = table_name;
    crt_table->columns = get_first_map_entry(columns);
    return crt_table;
}

drop_table_node *create_drop_table_node(char *table_name) {
    drop_table_node *drop_node = malloc(sizeof(struct drop_table_node));
    drop_node->table_name = table_name;
    return drop_node;
}

filter_node *create_filter_node(condition_union *condition) {
    filter_node *filter = malloc(sizeof(filter_node));
    filter->condition = condition;
    return filter;
}

subquery_node *create_subquery_node(void *statement, enum subquery_node_type subq_node_type) {
    subquery_node *subquery_node = malloc(sizeof(struct subquery_node));
    subquery_node->subquery_type = subq_node_type;
    subquery_node->statement = statement;
    subquery_node->prev = NULL;
    subquery_node->next = NULL;
    return subquery_node;
}

terminal_statement_node *create_terminal_statement_node(void *term_statement, enum AST_NODE_TYPE term_node_type) {
    terminal_statement_node *term_stmt_node = malloc(sizeof(struct terminal_statement_node));
    term_stmt_node->terminal_stmt_type = term_node_type;
    term_stmt_node->terminal_statement = term_statement;
    return term_stmt_node;
}

subquery_node *push_back_subquery(subquery_node *previous, subquery_node *new) {
    previous->next = new;
    new->prev = previous;
    return new;
}

subquery_node *get_first_subquery(subquery_node *last_subquery) {
    subquery_node *cur = NULL;
    while (last_subquery){
        cur = last_subquery;
        last_subquery = last_subquery->prev;
    }
    return cur;
}

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

return_node *create_return_node(void *value, enum return_node_type ret_node_type) {
    return_node *ret_node = malloc(sizeof(struct return_node));
    ret_node->node_type = AST_NODE_RETURN;
    ret_node->ret_node_type = ret_node_type;
    switch (ret_node_type) {
        case RETURN_CONSTANT:
            ret_node->constant = (struct constant *) value;
            break;
        case RETURN_MAP:
            ret_node->map = (map_entry *) value;
            break;
    }
    return ret_node;
}

map_entry *get_first_map_entry(map_entry *last_entry) {
    map_entry *cur = NULL;
    while (last_entry) {
        cur = last_entry;
        last_entry = last_entry->prev;
    }
    return cur;
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

