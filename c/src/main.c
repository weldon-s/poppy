#include <stdio.h>
#include <stdlib.h>

#include "codegen/assem.h"
#include "codegen/program.h"
#include "codegen/register.h"
#include "data/list.h"
#include "lang/builtin.h"
#include "lang/lexer.h"
#include "lang/parser.h"
#include "lang/poppy_grammar.h"
#include "lang/preprocess.h"
#include "lang/type.h"
#include "lang/typer.h"

char *intermediate_file = "inter.prog";

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
        if (argc < 2){
                printf("No input file passed, terminating...\n");
                exit(1);
        }

        preprocess(argv[1], intermediate_file);
        printf("preprocessed\n");
        FILE *in = fopen(intermediate_file, "r");
        struct LIST(token) *list = lex(in);
        fclose(in);
        remove(intermediate_file);

        if (list == NULL){
                return 0;
        }

        printf("lexed\n");

        const struct grammar *poppy_grammar = get_poppy_grammar();
        const struct parse_tree *pt = parse(poppy_grammar, list);
        free_poppy_grammar();

        if (pt == NULL) {
                free_list(list, free_token, token);
                free(list);
                return 0;
        }

        printf("parsed\n");

        const struct OUTER_TYPE_MAP *types = find_types(pt);
        if (types != NULL){
                printf("typed\n");
                char *code = generate_code(types, pt);
                if (code != NULL){
                        printf("generation successful\n");
                        FILE *out = fopen("../assembly/out.s", "w");
                        fprintf(out, "%s", code);
                        free(code);
                        fclose(out);
                }
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
