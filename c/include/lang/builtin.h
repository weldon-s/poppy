#ifndef BUILTIN_H
#define BUILTIN_H

#include "data/list.h"
#include "lang/type.h"
#include "lang/typer.h"

struct builtin {
        const struct type *type;
        char *name;
        char *(*evaluate)(char **args);
};

DEFINE_LIST(builtin);

char *evaluate_builtin(const struct builtin *builtin, char **args);
struct LIST(builtin) *get_builtins(struct LIST(string) modules);
void free_builtins(const struct LIST(builtin) *list);

#endif
