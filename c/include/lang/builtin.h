#ifndef BUILTIN_H
#define BUILTIN_H

#include "data/list.h"
#include "lang/type.h"

struct builtin {
        const struct type *type;
        char *name;
        char *body;
};

DEFINE_LIST(builtin);

struct LIST(builtin) *get_builtins();
void free_builtins();

#endif
