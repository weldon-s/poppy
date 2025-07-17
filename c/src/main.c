#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "poppy_grammar.h"

int main(int argc, char *argv[]){
        // FILE *f = fopen("prog.pop", "r");
        // struct token_list_node* head = lex(f);
        // struct token_list_node* cur = head;

        // while(cur != NULL){
        //         printf("%2d %s\n", cur->value->type, cur->value->value);
        //         cur = cur->next;
        // }

        // free_token_list(head);
        // fclose(f);

        const struct grammar * const poppy_grammar = get_poppy_grammar();
        free_poppy_grammar();
}
