#include <stdio.h>
#include <stdlib.h>


#include "data/list.h"
#include "lang/lexer.h"
#include "lang/parser.h"
#include "lang/poppy_grammar.h"
#include "lang/type.h"
#include "lang/typer.h"
#include "codegen/assem.h"
#include "codegen/register.h"


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

        const struct OUTER_TYPE_MAP *types = find_types(pt);

        printf("%p\n", types);

        free_list(list, free_token, token);
        free(list);
        free_parse_tree(pt);

        if (types != NULL){
                free_map(types, parse_tree, MAP(string, type));
                free((void*) types);
        }

        free_types();

        return 0;
}

// int main(){
//         char *c = concat(3, concat(7,
//                 mov(29, 3),
//                 movi(10, 300),
//                 push(1),
//                 push_pair(2, 3),
//                 pop(5),
//                 pop_pair(30, 31),
//                 add(1, 2, 3)),
//                 add(1, 2, 3),
//                 add(3, 2, 1)
//         );
//         printf("%s", c);
//         free(c);
// }
