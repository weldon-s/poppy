#ifndef PROGRAM_H
#define PROGRAM_H

#include "lang/parser.h"
#include "lang/typer.h"

struct program;

struct program *generate_code(struct OUTER_TYPE_MAP *type_map, struct parse_tree *tree);

#endif
