#include <stdio.h>
#include <stdlib.h>


#include "data/list.h"
#include "lang/builtin.h"
#include "lang/lexer.h"
#include "lang/parser.h"
#include "lang/poppy_grammar.h"
#include "lang/type.h"
#include "lang/typer.h"
#include "codegen/assem.h"
#include "codegen/program.h"
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
        FILE *in = fopen("prog.pop", "r");
        struct LIST(token) *list = lex(in);
        fclose(in);

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

        const struct OUTER_TYPE_MAP *types = find_types(pt);
        if (types != NULL){
                char *code = generate_code(types, pt);
                printf("generation successful\n");
                FILE *out = fopen("../assembly/out.s", "w");
                fprintf(out, "%s", code);
                free(code);
                fclose(out);
        }

        free_list(list, free_token, token);
        free(list);
        free_parse_tree(pt);

        if (types != NULL){
                free_map(types, parse_tree, MAP(string, type));
                free((void*) types);
        }

        free_types();
        // free_builtins();

        return 0;
}
