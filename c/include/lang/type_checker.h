#ifndef TYPE_CHECKER_H
#define TYPE_CHECKER_H

#include "data/map.h"
#include "lang/parser.h"

#define OUTER_TYPE_MAP parse_tree_symbol_table_entry_list_map

struct string {
        char *data;
};
bool equals_string(const struct string *s1, const struct string *s2);

struct symbol_table_entry {
        char *name;
        const struct type *type;
};
void free_symbol_table_entry(const struct symbol_table_entry *entry);

DEFINE_LIST(symbol_table_entry);

DEFINE_LIST(string)

// DEFINE_MAP(string, type);
DEFINE_MAP(parse_tree, LIST(symbol_table_entry));
// DEFINE_MAP(parse_tree, MAP(string, type));

struct OUTER_TYPE_MAP *find_types(const struct parse_tree *tree);
const struct type *find_symbol_type(const struct parse_tree *tree, const struct OUTER_TYPE_MAP *outer_map);
struct LIST(symbol_table_entry) get_local_variables(const struct parse_tree *tree, const struct OUTER_TYPE_MAP *symbols);
struct LIST(symbol_table_entry) get_parameters(const struct parse_tree *tree, const struct OUTER_TYPE_MAP *symbols);
#endif
