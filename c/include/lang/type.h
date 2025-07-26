#ifndef TYPER_H
#define TYPER_H
#include <stdbool.h>
#include <stdlib.h>

#define MAX_PARAM_COUNT 29
#define MAX_TYPE_STRING_LEN MAX_PARAM_COUNT + 3

struct type {
    char repr[MAX_TYPE_STRING_LEN];
    bool assignable;
};

const struct type* const int_type();
const struct type* const bool_type();
const struct type* const void_type();
const struct type* const function_type(const struct type *ret, const struct type *params[MAX_PARAM_COUNT], size_t params_len);
void free_types();
#endif
