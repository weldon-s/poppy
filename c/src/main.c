#include <stdio.h>

#include "lexer.h"

int main(int argc, char *argv[]){
        struct token_list_node* head = lex(stdin);
        struct token_list_node* cur = head;

        while(cur != NULL){
                printf("%d (%s)", cur->value->type, cur->value->value);
                cur = cur->next;
        }
}