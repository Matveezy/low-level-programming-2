#ifndef LOW_LEVEL_PROGRAMMING_2_STRUCT_H
#define LOW_LEVEL_PROGRAMMING_2_STRUCT_H

#include "stdbool.h"
#include "mm_malloc.h"

typedef enum predicate_type predicate_type;

#define operation(x) _Generic(x, \
enum enum constant_operation : "constant_op", \
enum enum log_operation : "log_operation")

enum CONSTANT_TYPE {
    FLOAT,
    INT,
    STRING,
    BOOL
};

struct constant {
    enum CONSTANT_TYPE const_type;
    bool is_string_ref;
    union {
        int int_data;
        float float_data;
        bool bool_data;
        char *string;
    };
};

enum log_operation {
    AND_OPERATION = 0,
    OR_OPERATION
};

enum constant_operation {
    GTE = 0,
    LTE,
    EQUALS,
    NEQUALS,
    GT,
    LT,
    LIKE
};

enum AST_NODE_TYPE {
    AST_NODE_FOR,
    AST_NODE_SELECT,
    AST_NODE_INSERT,
    AST_NODE_UPDATE,
    AST_NODE_REMOVE,
    AST_NODE_CONDITION,
    AST_NODE_FILTER,
    AST_NODE_RETURN,
};

struct map_entry {
    char *key;
    struct constant *constant;
    struct map_entry *next;
    struct map_entry *prev;
};

struct map_entry *create_map(char *, struct constant *);
struct map_entry *push_back_to_map(struct map_entry *, struct map_entry *);
#define create_map_entry(alias, attrname, predicate) _Generic(predicate, bool: create_map_entry_bool, char*: create_map_entry_char)(alias, attrname, predicate)
#endif //LOW_LEVEL_PROGRAMMING_2_STRUCT_H
