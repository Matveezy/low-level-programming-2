#ifndef LOW_LEVEL_PROGRAMMING_2_CONSTANT_H
#define LOW_LEVEL_PROGRAMMING_2_CONSTANT_H

#include "struct.h"
#include "mm_malloc.h"

struct constant *create_int_constant(int value);

struct constant *create_float_constant(float value);

struct constant *create_bool_constant(bool value);

struct constant *create_string_constant(char *value, bool is_ref);

//#define create_constant(value) _Generic(value, \
//int: create_int_constant,                      \
//float: create_float_constant,                  \
//bool: create_bool_constant,                    \
//char*: create_string_constant) (value)

#endif //LOW_LEVEL_PROGRAMMING_2_CONSTANT_H
