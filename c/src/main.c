#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "poppy_grammar.h"
#include "data/list.h"
#include "lang/parser.h"

int main(int argc, char *argv[]){
        FILE *f = fopen("prog.pop", "r");
        const struct grammar *poppy_grammar = get_poppy_grammar();

        struct LIST(token) *list = lex(f);
        const struct parse_tree *pt = parse(poppy_grammar, list);
        
        if(list == NULL){
                fclose(f);
                return 0;
        }

        for (struct LIST_NODE(token) *node = list->head; node != NULL; node = node->next){
                printf("%2d %s\n", node->data->type, node->data->value);

        }

        free_list(list, free_token, token);
        free(list);
        free_poppy_grammar(poppy_grammar);
        fclose(f);
        return 0;
}
