#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "poppy_grammar.h"
#include "data/list.h"

DEFINE_LIST(double)

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

        struct LIST(double) *lst = (struct LIST(double)*) malloc(sizeof(struct LIST(double)));
        init_list(lst)
        for (int i = 1; i <= 100; ++i){
                double *b = (double*) malloc(sizeof(double));
                *b = 1.0 / i;
                append_list(lst, b, double);
        }

        for (struct LIST_NODE(double) *node = lst->head; node != NULL; node = node->next) {
                printf("%f\n", *node->data);
        }

        free_list(lst, free, double);
}
