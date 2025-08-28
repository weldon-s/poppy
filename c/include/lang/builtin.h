#ifndef BUILTIN_H
#define BUILTIN_H

#include "data/list.h"
#include "lang/type.h"
#include "lang/typer.h"

struct builtin {
        const struct type *type;
        char *name;
        char *body;
};

DEFINE_LIST(builtin);

struct LIST(builtin) *get_builtins(struct LIST(string) modules);
void free_builtins();

#endif
