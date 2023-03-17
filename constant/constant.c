#include "constant.h"

struct constant *create_int_constant(int value) {
    struct constant *int_const = (struct constant *) malloc(sizeof(struct constant));
    int_const->const_type = INT;
    int_const->int_data = value;
    return int_const;
};

struct constant *create_float_constant(float value) {
    struct constant *float_const = (struct constant *) malloc(sizeof(struct constant));
    float_const->const_type = FLOAT;
    float_const->float_data = value;
    return float_const;
};

struct constant *create_bool_constant(bool value) {
    struct constant *bool_const = (struct constant *) malloc(sizeof(struct constant));
    bool_const->const_type = BOOL;
    bool_const->bool_data = value;
    return bool_const;
};

struct constant *create_string_constant(char *value, bool is_ref) {
    struct constant *string_const = (struct constant *) malloc(sizeof(struct constant));
    string_const->const_type = STRING;
    string_const->is_string_ref = is_ref;
    string_const->string = value;
    return string_const;
};