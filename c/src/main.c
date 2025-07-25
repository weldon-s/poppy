#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "poppy_grammar.h"
#include "data/list.h"
#include "lang/parser.h"

void print(const struct parse_tree *pt, size_t level){
        for(size_t i = 0; i < level; ++i){
                printf("  ");
        }

        printf("%s", symbol_name(pt->data.type));

        if (pt->data.value != NULL){
                printf(" %s", pt->data.value);
        }
        printf("\n");

        if (pt->children != NULL){
                for (struct LIST_NODE(parse_tree) *node = pt->children->head; node != NULL; node = node->next){
                        print(node->data, level + 1);
                }
        }
}

int main(int argc, char *argv[]){
        FILE *f = fopen("prog.pop", "r");
        struct LIST(token) *list = lex(f);
        fclose(f);

        if (list == NULL){
                return 0;
        }

        const struct grammar *poppy_grammar = get_poppy_grammar();
        const struct parse_tree *pt = parse(poppy_grammar, list);
        free_poppy_grammar();

        if (pt == NULL) {
                free_list(list, free_token, token);
                free(list);
                return 0;
        }

        print(pt, 0);

        free_list(list, free_token, token);
        free(list);
        free_parse_tree(pt);
        return 0;
}
