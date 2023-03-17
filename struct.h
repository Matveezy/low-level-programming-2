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
    AST_NODE_DELETE,
    AST_NODE_CONDITION,
    AST_NODE_FILTER
};

struct map_entry {
    char *alias;
    char *attrname;
    union {
        bool predicate;
        char *char_predicate;
    };
};

enum PREDICATE_TYPE {
    BOOL_TYPE,
    STRING_TYPE
};

struct map_entry *create_map_entry_bool(char *alias, char *attrname, bool predicate);

struct map_entry *create_map_entry_char(char *alias, char *attrname, char *predicate);

#define create_map_entry(alias, attrname, predicate) _Generic(predicate, bool: create_map_entry_bool, char*: create_map_entry_char)(alias, attrname, predicate)
#endif //LOW_LEVEL_PROGRAMMING_2_STRUCT_H
