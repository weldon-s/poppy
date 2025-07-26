#include <stdio.h>
#include <stdlib.h>


#include "data/map.h"
#include "data/list.h"
#include "lang/lexer.h"
#include "lang/parser.h"
#include "lang/poppy_grammar.h"

// void print(const struct parse_tree *pt, size_t level){
//         for(size_t i = 0; i < level; ++i){
//                 printf("  ");
//         }

//         printf("%s", symbol_name(pt->data.type));

//         if (pt->data.value != NULL){
//                 printf(" %s", pt->data.value);
//         }
//         printf("\n");

//         if (pt->children != NULL){
//                 for (struct LIST_NODE(parse_tree) *node = pt->children->head; node != NULL; node = node->next){
//                         print(node->data, level + 1);
//                 }
//         }
// }

// int main(int argc, char *argv[]){
//         FILE *f = fopen("prog.pop", "r");
//         struct LIST(token) *list = lex(f);
//         fclose(f);

//         if (list == NULL){
//                 return 0;
//         }

//         const struct grammar *poppy_grammar = get_poppy_grammar();
//         const struct parse_tree *pt = parse(poppy_grammar, list);
//         free_poppy_grammar();

//         if (pt == NULL) {
//                 free_list(list, free_token, token);
//                 free(list);
//                 return 0;
//         }

//         print(pt, 0);

//         free_list(list, free_token, token);
//         free(list);
//         free_parse_tree(pt);
//         return 0;
// }

struct i {
        int num;
};

struct d {
        double num;
};

DEFINE_MAP(i, d);

bool equals_i(struct i *i1, struct i *i2){
        return i1->num == i2->num;
}

void free_entry(struct MAP_ENTRY(i, d) *entry){
        free(entry->key);
        free(entry->value);
        free(entry);
}

int main (int argc, char *argv[]){
        struct MAP(i, d) map;
        init_map((&map), equals_i, free_entry, i, d);

        for (int i = 1; i <= 10; ++i){
                struct i *key = (struct i*) malloc(sizeof(struct i));
                key->num = i;
                struct d *value = (struct d*) malloc(sizeof(struct d));
                value->num = 1.0 / i;
                update_map((&map), key, value, i, d);
        }

        struct i *key = (struct i*) malloc(sizeof(struct i));
        struct d *value = (struct d*) malloc(sizeof(struct d));
        key->num = 5;
        value->num = 0.5;
        update_map((&map), key, value, i, d);
        key = (struct i*) malloc(sizeof(struct i));
        value = (struct d*) malloc(sizeof(struct d));
        key->num = 12;
        value->num = 1;
        update_map((&map), key, value, i, d);

        for (int i = 1; i <= 12; ++i){
                struct i c = {i};
                struct d *r;
                query_map((&map), (&c), r, i, d);
                r ? printf("%f\n", r->num) : printf("null\n");
        }

        free_map((&map), i, d);
}