#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdbool.h>
#include <stddef.h>

#include "codegen/register.h"

struct function;

struct function *new_function(char **params, size_t params_len, char **vars, size_t vars_len, bool is_main);
void free_function(const struct function* function);

size_t num_params(const struct function *function);
char *read_function_variable(const struct function *function, enum reg reg, char *varname);
char *write_function_variable(const struct function *function, char *varname, enum reg reg);
void set_body(struct function *function, char *body);
char *declare_function(const struct function *function);
char *call_function(const struct function *function, char **args);
char *hop(const struct function *function);
#endif
