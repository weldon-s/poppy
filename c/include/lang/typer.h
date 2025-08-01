#ifndef TYPER_H
#define TYPER_H

#include "data/map.h"
#include "lang/parser.h"

#define OUTER_MAP parse_tree_string_type_map_map

struct string {
        char *data;
};

DEFINE_MAP(string, type);
DEFINE_MAP(parse_tree, MAP(string, type));

struct OUTER_MAP * find_types(const struct parse_tree *tree);

#endif
