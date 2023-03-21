#include "ast.h"

char *query_node_type_to_string(enum query_node_type query_node_type);

char *union_type_to_string(enum condition_union_type cond_union_type);

char *const_type_to_string(enum CONSTANT_TYPE const_type);

char *const_op_type_to_string(enum constant_operation cons_op);

void print_const(struct constant *left_const);

void print_simple_condition(condition_union *simple_cond_union, int);

map_entry *get_first_map_entry(map_entry *last_entry);

subquery_node *get_first_subquery(subquery_node *last_subquery);

void print_key_value(char *key, char *value, int depth);

void print_drop_table_node(drop_table_node *drop_table, int depth);

void print_constant(struct constant *constant, int depth);

void print_schema_entry(map_entry *map_entry, int depth);

void print_table_schema(map_entry *map, int depth);

void print_create_table_node(create_node *create_table_node, int depth);

void print_int_value(int value, int depth);

void print_bool_value(bool value, int depth);

void print_float_value(float value, int depth);

void print_constant_in_map(struct constant *constant, int depth);

void print_map(map_entry *map, int depth);

void print_insert_query(insert_node *ins_node, int depth);

char *ast_node_type_to_string(enum AST_NODE_TYPE ast_node_type);

void print_remove_statement(remove_node *remove_node, int depth);

void print_subqueries(subquery_node *subq_node, int depth);

void print_for_query(for_node *for_query, int depth);

void print_update_statement(update_node *upd_node, int depth);

void print_filter_statement(filter_node *filter_node, int depth);

void print_return_statement(return_node *ret_node, int depth);

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
    while (last_subquery) {
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
            ret_node->map = get_first_map_entry((map_entry *) value);
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

void print_ast(query_node *query_node, int depth) {
    switch (query_node->query_node_type) {
        case FOR_QUERY_STATEMENT:
            print_for_query((for_node *) query_node->statement, depth);
            break;
        case INSERT_QUERY_STATEMENT:
            print_insert_query((insert_node *) query_node->statement, depth);
            break;
        case CREATE_QUERY_STATEMENT:
            print_create_table_node((create_node *) query_node->statement, depth);
            break;
        case DROP_QUERY_STATEMENT:
            print_drop_table_node((drop_table_node *) query_node->statement, depth);
            break;
    }
}

void print_for_query(for_node *for_query, int depth) {
    print_key_value("node_type", query_node_type_to_string(FOR_QUERY_STATEMENT), depth);
    print_key_value("table", for_query->table_name, depth);
    print_key_value("var", for_query->alias, depth);
    print_key_value("subqueries", "", depth);
    print_subqueries(for_query->subquery_list, depth + 1);
}

void print_terminal_statement(terminal_statement_node *term_node, int depth) {
    print_key_value("node_type", ast_node_type_to_string(term_node->terminal_stmt_type), depth);
    switch (term_node->terminal_stmt_type) {
        case AST_NODE_REMOVE:
            print_remove_statement((remove_node *) term_node->terminal_statement, depth + 1);
            break;
        case AST_NODE_UPDATE:
            print_update_statement((update_node *) term_node->terminal_statement, depth + 1);
            break;
        case AST_NODE_RETURN:
            print_return_statement((return_node *) term_node->terminal_statement, depth + 1);
            break;
    }
}

void print_return_statement(return_node *ret_node, int depth) {
    print_key_value("return_value", "", depth);
    switch (ret_node->ret_node_type) {
        case RETURN_CONSTANT:
            print_constant_in_map(ret_node->constant, depth + 1);
            break;
        case RETURN_MAP:
            print_map(ret_node->map, depth + 1);
            break;
    }
}

void print_update_statement(update_node *upd_node, int depth) {
    print_key_value("table", upd_node->table_name, depth);
    print_key_value("variable", upd_node->variable, depth);
    print_map(upd_node->map, depth + 1);
}

void print_remove_statement(remove_node *remove_node, int depth) {
    print_key_value("table", remove_node->table_name, depth);
    print_key_value("variable", remove_node->variable, depth);
}

void print_condition_union(condition_union *cond_union, int depth) {
    switch (cond_union->union_type) {
        case SIMPLE_CONDITION:
            print_simple_condition(cond_union, depth);
            break;
        case UNION_CONDITION:
            break;
    }
}

void print_simple_condition(condition_union *simple_cond_union, int depth) {
    print_key_value("condition_type", union_type_to_string(simple_cond_union->union_type), depth);
    print_key_value("operation",
                    const_op_type_to_string(simple_cond_union->left_side.left_condition_node->constant_op), depth);
    print_key_value("left_value", "", depth + 1);
    print_constant_in_map(simple_cond_union->left_side.left_condition_node->left_value, depth + 2);
    print_key_value("right_value", "", depth + 1);
    print_constant_in_map(simple_cond_union->left_side.left_condition_node->right_value, depth + 2);
}

void print_filter_statement(filter_node *filter_node, int depth) {
    print_key_value("node_type", "filter", depth);
    print_condition_union(filter_node->condition, depth + 1);
}

void print_subqueries(subquery_node *subq_node, int depth) {
    while (subq_node) {
        print_key_value("subquery", "", depth);
        switch (subq_node->subquery_type) {
            case FOR_STATEMENT:
                print_for_query((for_node *) subq_node->statement, depth + 1);
                break;
            case FILTER_STATEMENT:
                print_filter_statement((filter_node *) subq_node->statement, depth + 1);
                break;
            case TERMINAL_STATEMENT:
                print_terminal_statement((terminal_statement_node *) subq_node->statement, depth + 1);
                break;
        }
        subq_node = subq_node->next;
    }
}

void print_insert_query(insert_node *ins_node, int depth) {
    print_key_value("node_type", query_node_type_to_string(INSERT_QUERY_STATEMENT), depth);
    print_key_value("table", ins_node->tablename, depth);
    print_key_value("values", "", depth);
    print_map(ins_node->map, depth + 1);
}

void print_create_table_node(create_node *create_table_node, int depth) {
    print_key_value("node_type", query_node_type_to_string(CREATE_QUERY_STATEMENT), depth);
    print_key_value("table", create_table_node->table_name, depth);
    print_key_value("schema", "", depth);
    print_table_schema(create_table_node->columns, depth + 1);
}

void print_table_schema(map_entry *map, int depth) {
    print_key_value("node_type", "map", depth);
    print_key_value("entries", "", depth);
    while (map) {
        print_key_value("entry", "", depth + 1);
        print_schema_entry(map, depth + 2);
        map = map->next;
    }
}

void print_map(map_entry *map, int depth) {
    print_key_value("node_type", "map", depth);
    print_key_value("entries", "", depth);
    while (map) {
        print_key_value("entry", "", depth + 1);
        print_key_value("key", map->key, depth + 2);
        print_constant_in_map(map->constant, depth + 2);
        map = map->next;
    }
}

void print_schema_entry(map_entry *map_entry, int depth) {
    print_key_value("node_type", "map_entry", depth);
    print_key_value("key", map_entry->key, depth);
    print_key_value("value", "", depth);
    print_constant(map_entry->constant, depth + 1);
}

void print_constant(struct constant *constant, int depth) {
    print_key_value("node_type", "constant", depth);
    print_key_value("const_type", constant->is_string_ref ? "reference" : "value", depth);
    print_key_value("const_data_type", constant->string, depth);
}

void print_constant_in_map(struct constant *constant, int depth) {
    print_key_value("node_type", "constant", depth);
    switch (constant->const_type) {
        case FLOAT:
            print_key_value("const_data_type", "float", depth);
            print_float_value(constant->float_data, depth);
            break;
        case INT:
            print_key_value("const_data_type", "int", depth);
            print_int_value(constant->int_data, depth);
            break;
        case STRING:
            print_key_value("const_data_type", "string", depth);
            print_key_value("value", constant->string, depth);
            break;
        case BOOL:
            print_key_value("const_data_type", "bool", depth);
            print_bool_value(constant->bool_data, depth);
            break;
    }
}


void print_drop_table_node(drop_table_node *drop_table, int depth) {
    print_key_value("node_type", query_node_type_to_string(DROP_QUERY_STATEMENT), depth);
    print_key_value("table", drop_table->table_name, depth);
}

void print_key_value(char *key, char *value, int depth) {
    for (int i = 0; i < depth; i++) {
        printf("   ");
    }
    printf("%s: %s\n", key, value);
}

void print_bool_value(bool value, int depth) {
    for (int i = 0; i < depth; i++) {
        printf("   ");
    }
    printf("%s: %s\n", "value", value ? "true" : "false");
}

void print_int_value(int value, int depth) {
    for (int i = 0; i < depth; i++) {
        printf("   ");
    }
    printf("%s: %d\n", "value", value);
}

void print_float_value(float value, int depth) {
    for (int i = 0; i < depth; i++) {
        printf("   ");
    }
    printf("%s: %f\n", "value", value);
}

char *query_node_type_to_string(enum query_node_type query_node_type) {
    switch (query_node_type) {
        case FOR_QUERY_STATEMENT:
            return "for_query";
        case INSERT_QUERY_STATEMENT:
            return "insert_query";
        case CREATE_QUERY_STATEMENT:
            return "create_table_query";
        case DROP_QUERY_STATEMENT:
            return "drop_table_query";
    }
}

char *union_type_to_string(enum condition_union_type cond_union_type) {
    if (cond_union_type == SIMPLE_CONDITION) {
        return "simple_condition";
    }
    return "condition_union";
}

char *ast_node_type_to_string(enum AST_NODE_TYPE ast_node_type) {
    switch (ast_node_type) {
        case AST_NODE_FOR:
            return "for";
        case AST_NODE_SELECT:
            return "select";
        case AST_NODE_INSERT:
            return "insert";
        case AST_NODE_UPDATE:
            return "update";
        case AST_NODE_REMOVE:
            return "remove";
        case AST_NODE_CONDITION:
            return "condition";
        case AST_NODE_FILTER:
            return "filter";
        case AST_NODE_RETURN:
            return "return";
    }
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


