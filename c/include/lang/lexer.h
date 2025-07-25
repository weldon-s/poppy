#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

#include "data/list.h"
#include "lang/symbol.h"

struct token {
        enum symbol type;
        char *value;
};

void free_token(struct token *t);

DEFINE_LIST(token);
struct LIST(token) * lex(FILE *file);

#endif