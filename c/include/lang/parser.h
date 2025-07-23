#ifndef PARSER_H
#define PARSER_H

#include "data/list.h"
#include "grammar.h"
#include "lexer.h"

DEFINE_LIST(parse_tree);

struct parse_tree {
        struct token data;
        struct LIST(parse_tree) * children;
        struct parse_tree *parent;
};

const struct parse_tree * const parse(const struct grammar *grammar, const struct LIST(token) *head);

#endif
