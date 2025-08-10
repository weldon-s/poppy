#ifndef FUNCTION_H
#define FUNCTION_H

#include <stddef.h>

#include "codegen/register.h"

struct function;

struct function *new_function(char **params, size_t params_len);
void free_function(struct function* function);

char *read_function_variable(struct function *function, enum reg reg, char *varname);
char *write_function_variable(struct function *function, char *varname, enum reg reg);
void set_body(struct function *function, char *body);
char *declare_function(struct function *function);
char *call_function(struct function *function, char **args);

#endif
