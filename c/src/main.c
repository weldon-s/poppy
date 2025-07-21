#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "poppy_grammar.h"
#include "data/list.h"
#include "lang/parser.h"

int main(int argc, char *argv[]){
        FILE *f = fopen("prog.pop", "r");
        struct LIST(token) *list = lex(f);
        
        if(list == NULL){
                fclose(f);
                return 0;
        }

        for (struct LIST_NODE(token) *node = list->head; node != NULL; node = node->next){
                printf("%2d %s\n", node->data->type, node->data->value);

        }

        free_list(list, free_token, token);
        fclose(f);
        return 0;
}
