#ifndef PROGRAM_H
#define PROGRAM_H

#include "lang/parser.h"
#include "lang/typer.h"

char *generate_code(const struct OUTER_TYPE_MAP *type_map, const struct parse_tree *tree);

#endif
