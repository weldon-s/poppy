#include <stdio.h>

#include "symbol.h"

struct token {
        enum symbol type;
        char *value;
};

struct token_list_node;
struct token_list_node {
        struct token *value;
        struct token_list_node *next;
};

struct token_list_node* lex(FILE *file);

void free_token_list(struct token_list_node* head);