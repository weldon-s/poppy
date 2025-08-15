#include "codegen/program.h"

#include "codegen/function.h"


#define load_child_at(var, tree, n)                                        \
        do {                                                               \
                struct LIST_NODE(parse_tree) *node = tree->children->head; \
                for (int i = 0; i < n; ++i){                               \
                        node = node->next;                                 \
                }                                                          \
                var = node->data;                                          \
        } while (0);                                                       \

struct varname {
        char *data;
};

DEFINE_LIST(varname);
DEFINE_MAP(string, function);

void free_string_function_entry(const struct MAP_ENTRY(string, function) *entry) {
        free_function(entry->key);
        free_function(entry->value);
        free((void*) entry);
}

char *generate_function_body(struct parse_tree *tree, struct OUTER_TYPE_MAP *type_map, struct MAP(string, function) *functions){
        
}

struct program *generate_code(struct OUTER_TYPE_MAP *type_map, struct parse_tree *tree){
        struct MAP(string, function) functions; init_map((&functions), equals_string, free_string_function_entry, string, function);
        // program -> optincludes defns END
        struct parse_tree *defns; load_child_at(defns, tree, 1);

        while (1) {
                // defns -> defn defns
                // defns -> defn
                struct parse_tree *defn = defns->children->head->data;

                struct LIST(string) params_list = get_parameters(defn, type_map);
                char **params = (char**) malloc(params_list.len * sizeof(char*));
                size_t i = 0;
                for (struct LIST_NODE(string) *node = params_list.head; node != NULL; node = node->next){
                        params[i++] = node->data->data;
                }

                struct LIST(string) locals_list = get_local_variables(defn, type_map);
                char **locals = (char**) malloc(locals_list.len * sizeof(char*));
                i = 0;
                for (struct LIST_NODE(string) *node = locals_list.head; node != NULL; node = node->next){
                        locals[i++] = node->data->data;
                }

                struct function *fn = new_function(params, params_list.len, locals, locals_list.len);
                struct string *s = (struct string*) malloc(sizeof(struct string));
                s->data = defn->children->head->next->data->data.value;
                update_map((&functions), s, fn, string, function);

                if (defns->children->len == 2){
                        load_child_at(defns, defns, 1);
                } else {
                        break;
                }
        }

        load_child_at(defns, tree, 1);

        while (1) {
                // defns -> defn defns
                // defns -> defn
                struct parse_tree *defn = defns->children->head->data;

                struct LIST(string) params_list = get_parameters(defn, type_map);
                char **params = (char**) malloc(params_list.len * sizeof(char*));
                size_t i = 0;
                for (struct LIST_NODE(string) *node = params_list.head; node != NULL; node = node->next){
                        params[i++] = node->data->data;
                }

                struct LIST(string) locals_list = get_local_variables(defn, type_map);
                char **locals = (char**) malloc(locals_list.len * sizeof(char*));
                i = 0;
                for (struct LIST_NODE(string) *node = locals_list.head; node != NULL; node = node->next){
                        locals[i++] = node->data->data;
                }

                struct function *fn = new_function(params, params_list.len, locals, locals_list.len);
                struct string *s = (struct string*) malloc(sizeof(struct string));
                s->data = defn->children->head->next->data->data.value;
                update_map((&functions), s, fn, string, function);

                if (defns->children->len == 2){
                        load_child_at(defns, defns, 1);
                } else {
                        break;
                }
        }
}
