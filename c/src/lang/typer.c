#include "lang/typer.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "data/map.h"
#include "lang/type.h"

#define OUTER_TYPE_MAP_ENTRY parse_tree_string_type_map_map_entry

#define load_child_at(var, tree, n)                                        \
        do {                                                               \
                struct LIST_NODE(parse_tree) *node = tree->children->head; \
                for (int i = 0; i < n; ++i){                               \
                        node = node->next;                                 \
                }                                                          \
                var = node->data;                                          \
        } while (0);                                                       \

#define verify_type(tree, st) assert(tree->data.type == st);

const struct type * params_array[MAX_PARAM_COUNT];

bool equals_string(const struct string *s1, const struct string *s2) {
        return strcmp(s1->data, s2->data) == 0;
}

void free_string(const struct MAP_ENTRY(string, type) *entry){
        free((void *) entry->key);
        free((void *) entry);
}

bool equals_parse_tree(const struct parse_tree *pt1, const struct parse_tree *pt2){
        return pt1 == pt2;
}

void free_typer_entry(const struct OUTER_TYPE_MAP_ENTRY *entry){
        free_map(entry->value, string, type);
        free((void *) entry->value);
        free((void *) entry);
}

struct MAP(string, type) * new_inner_map() {
        struct MAP(string, type) *ptr = (struct MAP(string, type)*) malloc(sizeof(struct MAP(string, type)));
        init_map(ptr, equals_string, free_string, string, type);
        return ptr;
}

const struct type * find_stmts_type(struct parse_tree *tree, struct OUTER_TYPE_MAP *outer_map, struct MAP(string, type) *scope_map);
const struct type * find_cond_type(struct parse_tree *tree, struct OUTER_TYPE_MAP *outer_map);
const struct type * find_expr_type(struct parse_tree *tree, struct OUTER_TYPE_MAP *outer_map);

const struct type * find_type_type(struct parse_tree *tree){
        verify_type(tree, SYMBOL_TYPE);
        // type -> INT
        struct parse_tree *child = tree->children->head->data;

        if (strcmp(child->data.value, "int") == 0){
                return int_type();
        }

        return NULL;
}

const struct type * find_symbol_type(struct parse_tree *tree, struct OUTER_TYPE_MAP *outer_map){
        struct string string;
        string.data = tree->data.value;
        struct parse_tree *cur = tree;
        while (cur != NULL){
                const struct MAP(string, type) *inner_map; query_map(outer_map, cur, inner_map, parse_tree, MAP(string, type));
                if (inner_map != NULL){
                        const struct type *type; query_map(inner_map, &string, type, string, type);

                        if (type != NULL){
                                return type;
                        }
                }
                cur = cur->parent;
        }
        return NULL;
}

char * find_defn_name(struct parse_tree *tree){
        struct parse_tree *identifier; load_child_at(identifier, tree, 1);
        return identifier->data.value;
}

const struct type * find_defn_type(struct parse_tree *tree, struct MAP(string, type) *scope_map){
        verify_type(tree, SYMBOL_DEFN);
        // defn -> type IDENTIFIER LPAREN optparams RPAREN LBRACE stmts RBRACE
        const struct type * ret = find_type_type(tree->children->head->data);

        struct parse_tree *optparams; load_child_at(optparams, tree, 3);
        if ((optparams->children == NULL) || (optparams->children->len == 0)){
                // optparams ->
                return function_type(ret, params_array, 0);
        }

        // optparams -> params
        struct parse_tree *params = optparams->children->head->data;
        size_t param_count = 0;

        while (1) {
                // params -> param COMMA params
                // params -> param
                struct parse_tree *param = params->children->head->data;

                // param -> type IDENTIFIER
                params_array[param_count] = find_type_type(param->children->head->data);
                struct string *s = (struct string*) malloc(sizeof(struct string));
                s->data = param->children->head->next->data->data.value;
                update_map(scope_map, s, params_array[param_count], string, type)
                ++param_count;


                if (params->children->len == 3){
                        load_child_at(params, params, 2);
                } else {
                        break;
                }
        };

        return function_type(ret, params_array, param_count);
}

const struct type * find_vardec_type(struct parse_tree *tree, struct OUTER_TYPE_MAP *outer_map, struct MAP(string, type) *scope_map){
        verify_type(tree, SYMBOL_VARDEC);
        // vardec -> LET type IDENTIFIER ASSIGN expr
        // vardec -> LET type IDENTIFIER
        struct parse_tree *type; load_child_at(type, tree, 1);
        const struct type *type_type = find_type_type(type);
        if (type_type == NULL){
                return NULL;
        }

        if (tree->children->len == 5){
                struct parse_tree *expr; load_child_at(expr, tree, 4);
                const struct type *expr_type = find_expr_type(expr, outer_map);
                if ((expr_type == NULL) || (!equals_type(type_type, expr_type))){
                        return NULL;
                }
        }

        struct parse_tree *id; load_child_at(id, tree, 2);
        struct string *str = (struct string*) malloc(sizeof(struct string));
        str->data = id->data.value;
        update_map(scope_map, str, type_type, string, type);
        return void_type();
}

const struct type * find_varasst_type(struct parse_tree *tree, struct OUTER_TYPE_MAP *outer_map){
        verify_type(tree, SYMBOL_VARASST);
        // varasst -> IDENTIFIER ASSIGN expr
        struct parse_tree *id = tree->children->head->data;
        const struct type *var_type = find_symbol_type(id, outer_map);
        if (var_type == NULL){
                return NULL;
        }

        struct parse_tree *expr; load_child_at(expr, tree, 2);
        const struct type *expr_type = find_expr_type(expr, outer_map);
        if ((expr_type == NULL) || !equals_type(var_type, expr_type)){
                return NULL;
        }

        return void_type();
}

const struct type * find_ret_type(struct parse_tree *tree, struct OUTER_TYPE_MAP *outer_map){
        verify_type(tree, SYMBOL_RET);
        // ret -> HOP expr
        struct parse_tree *expr; load_child_at(expr, tree, 1);
        return find_expr_type(expr, outer_map);
}

const struct type * find_call_type(struct parse_tree *tree, struct OUTER_TYPE_MAP *outer_map){
        verify_type(tree, SYMBOL_UNEXPR);
        // unexpr -> IDENTIFIER LPAREN optargs RPAREN
        struct parse_tree *optargs; load_child_at(optargs, tree, 2);

        // optargs ->
        size_t param_count = 0;
        if ((optargs->children != NULL) && (optargs->children->len > 0)){
                // optargs -> args
                struct parse_tree *args = optargs->children->head->data;
                while (1) {
                        // args -> expr COMMA args
                        // args -> expr
                        struct parse_tree *expr = args->children->head->data;

                        params_array[param_count] = find_expr_type(expr, outer_map);
                        if (params_array[param_count] == NULL){
                                return NULL;
                        }

                        ++param_count;

                        if (args->children->len == 3){
                                load_child_at(args, args, 2);
                        } else {
                                break;
                        }
                };
        }

        const struct type *ftype = find_symbol_type(tree->children->head->data, outer_map);
        if (equals_arg_types(params_array, param_count, ftype)){
                return return_type(ftype);
        }
        return NULL;
}

const struct type * find_unexpr_type(struct parse_tree *tree, struct OUTER_TYPE_MAP *outer_map){
        verify_type(tree, SYMBOL_UNEXPR);
        struct parse_tree *head = tree->children->head->data;

        switch (head->data.type){
                case SYMBOL_MINUS:
                        // unexpr -> MINUS UNEXPR
                        struct parse_tree *unexpr; load_child_at(unexpr, tree, 1);
                        const struct type *type = find_unexpr_type(unexpr, outer_map);
                        return type && equals_type(type, int_type()) ? int_type() : NULL;
                case SYMBOL_LPAREN:
                        // unexpr -> LPAREN expr RPAREN
                        struct parse_tree *expr; load_child_at(expr, tree, 1);
                        return find_expr_type(expr, outer_map);
                case SYMBOL_INC:
                case SYMBOL_DEC:
                        // unexpr -> INC IDENTIFIER
                        // unexpr -> DEC IDENTIFIER
                        struct parse_tree *id; load_child_at(id, tree, 1);
                        const struct type *id_type = find_symbol_type(id, outer_map);
                        return id_type && equals_type(id_type, int_type()) ? int_type() : NULL;
                case SYMBOL_IDENTIFIER:
                        if (tree->children->len == 1){
                                // unexpr -> IDENTIFIER
                                return find_symbol_type(head, outer_map);
                        }
                        // unexpr -> IDENTIFIER LPAREN optargs RPAREN
                        return find_call_type(tree, outer_map);

                case SYMBOL_CONSTANT:
                        // unexpr -> CONSTANT
                        return int_type();

                default:
                        return NULL;
        }
}

const struct type * find_multexpr_type(struct parse_tree *tree, struct OUTER_TYPE_MAP *outer_map){
        verify_type(tree, SYMBOL_MULTEXPR);
        // multexpr -> unexpr
        if (tree->children->len == 1){
                return find_unexpr_type(tree->children->head->data, outer_map);
        }

        // multexpr -> multexpr TIMES unexpr
        // multexpr -> multexpr DIVIDE unexpr
        // multexpr -> multexpr MOD unexpr
        struct parse_tree *op1; load_child_at(op1, tree, 0);
        const struct type *op1_type = find_multexpr_type(op1, outer_map);
        if ((op1_type == NULL) || !equals_type(op1_type, int_type())){
                return NULL;
        }

        struct parse_tree *op2; load_child_at(op2, tree, 2);
        const struct type *op2_type = find_unexpr_type(op2, outer_map);
        if ((op2_type == NULL) || !equals_type(op2_type, int_type())){
                return NULL;
        }

        return int_type();
}

const struct type * find_addexpr_type(struct parse_tree *tree, struct OUTER_TYPE_MAP *outer_map){
        verify_type(tree, SYMBOL_ADDEXPR);
        // addexpr -> multexpr
        if (tree->children->len == 1){
                return find_multexpr_type(tree->children->head->data, outer_map);
        }

        // addexpr -> addexpr PLUS multexpr
        // addexpr -> addexpr MINUS multexpr
        struct parse_tree *op1; load_child_at(op1, tree, 0);
        const struct type *op1_type = find_addexpr_type(op1, outer_map);
        if ((op1_type == NULL) || !equals_type(op1_type, int_type())){
                return NULL;
        }

        struct parse_tree *op2; load_child_at(op2, tree, 2);
        const struct type *op2_type = find_multexpr_type(op2, outer_map);
        if ((op2_type == NULL) || !equals_type(op2_type, int_type())){
                return NULL;
        }

        return int_type();
}

const struct type * find_expr_type(struct parse_tree *tree, struct OUTER_TYPE_MAP *outer_map){
        verify_type(tree, SYMBOL_EXPR);
        // expr -> addexpr
        return find_addexpr_type(tree->children->head->data, outer_map);
}

const struct type * find_semistmt_type(struct parse_tree *tree, struct OUTER_TYPE_MAP *outer_map, struct MAP(string, type) *scope_map){
        verify_type(tree, SYMBOL_SEMISTMT);
        struct parse_tree *child = tree->children->head->data;
        switch (child->data.type){
                case SYMBOL_VARDEC:
                        // semistmt -> vardec
                        return find_vardec_type(child, outer_map, scope_map);
                case SYMBOL_VARASST:
                        // semistmt -> varasst
                        return find_varasst_type(child, outer_map);
                case SYMBOL_RET:
                        // semistmt -> ret
                        return find_ret_type(child, outer_map);
                case SYMBOL_EXPR:
                        // semistmt -> expr
                        return find_expr_type(child, outer_map);
                default:
                        return NULL;
        }
}

const struct type * find_uncond_type(struct parse_tree *tree, struct OUTER_TYPE_MAP *outer_map){
        verify_type(tree, SYMBOL_UNCOND);
        struct parse_tree *head = tree->children->head->data;

        if ((head->data.type == SYMBOL_NOT) || (head->data.type == SYMBOL_LPAREN)){
                        // uncond -> NOT cond
                        // uncond -> LPAREN cond RPAREN
                        struct parse_tree *cond; load_child_at(cond, tree, 1);
                        return find_cond_type(cond, outer_map);
        }

        // uncond -> expr LT expr
        // uncond -> expr GT expr
        // uncond -> expr LE expr
        // uncond -> expr GE expr
        // uncond -> expr EQ expr
        // uncond -> expr NE expr
        struct parse_tree *op1; load_child_at(op1, tree, 0);
        const struct type *op1_type = find_expr_type(op1, outer_map);
        if ((op1_type == NULL) || !equals_type(op1_type, int_type())){
                return NULL;
        }

        struct parse_tree *op2; load_child_at(op2, tree, 0);
        const struct type *op2_type = find_expr_type(op2, outer_map);
        if ((op2_type == NULL) || !equals_type(op2_type, int_type())){
                return NULL;
        }

        return bool_type();                 
}

const struct type * find_orcond_type(struct parse_tree *tree, struct OUTER_TYPE_MAP *outer_map){
        verify_type(tree, SYMBOL_ORCOND);
        if (tree->children->len == 1){
                // orcond -> uncond
                return find_uncond_type(tree->children->head->data, outer_map);
        }

        // orcond -> orcond OR uncond
        struct parse_tree *op1; load_child_at(op1, tree, 0);
        const struct type *op1_type = find_orcond_type(op1, outer_map);
        if ((op1_type == NULL) || !equals_type(op1_type, bool_type())){
                return NULL;
        }

        struct parse_tree *op2; load_child_at(op2, tree, 2);
        const struct type *op2_type = find_uncond_type(op2, outer_map);
        if ((op2_type == NULL) || !equals_type(op2_type, bool_type())){
                return NULL;
        }

        return bool_type();
}

const struct type * find_andcond_type(struct parse_tree *tree, struct OUTER_TYPE_MAP *outer_map){
        verify_type(tree, SYMBOL_ANDCOND);
        if (tree->children->len == 1){
                // andcond -> orcond
                return find_orcond_type(tree->children->head->data, outer_map);
        }

        // andcond -> andcond AND orcond
        struct parse_tree *op1; load_child_at(op1, tree, 0);
        const struct type *op1_type = find_andcond_type(op1, outer_map);
        if ((op1_type == NULL) || !equals_type(op1_type, bool_type())){
                return NULL;
        }

        struct parse_tree *op2; load_child_at(op2, tree, 2);
        const struct type *op2_type = find_orcond_type(op2, outer_map);
        if ((op2_type == NULL) || !equals_type(op2_type, bool_type())){
                return NULL;
        }

        return bool_type();
}

const struct type * find_cond_type(struct parse_tree *tree, struct OUTER_TYPE_MAP *outer_map){
        verify_type(tree, SYMBOL_COND);
        // cond -> andcond
        return find_andcond_type(tree->children->head->data, outer_map);
}

const struct type * find_if_type(struct parse_tree *tree, struct OUTER_TYPE_MAP *outer_map){
        verify_type(tree, SYMBOL_STMT);
        // stmt -> IF LPAREN cond RPAREN LBRACE stmts RBRACE optelse
        struct parse_tree *if_stmts; load_child_at(if_stmts, tree, 5);
        struct MAP(string, type) *if_map = new_inner_map();
        update_map(outer_map, if_stmts, if_map, parse_tree, MAP(string, type));

        const struct type *if_type = find_stmts_type(if_stmts, outer_map, if_map);
        if (if_type == NULL){
                return NULL;
        }

        struct parse_tree *optelse; load_child_at(optelse, tree, 7);
        // optelse ->
        if ((optelse->children == NULL) || (optelse->children->len == 0)) {
                return void_type();
        }

        // optelse -> ELSE LBRACE stmts RBRACE
        struct parse_tree *else_stmts; load_child_at(else_stmts, optelse, 2);
        struct MAP(string, type) *else_map = new_inner_map();
        update_map(outer_map, else_stmts, else_map, parse_tree, MAP(string, type));
        const struct type *else_type = find_stmts_type(else_stmts, outer_map, else_map);

        if ((else_type == NULL) || (!equals_type(if_type, else_type))){
                return NULL;
        }
        return if_type;
}

const struct type * find_while_type(struct parse_tree *tree, struct OUTER_TYPE_MAP *outer_map){
        verify_type(tree, SYMBOL_STMT);
        struct parse_tree *cond; load_child_at(cond, tree, 2);
        const struct type *cond_type = find_cond_type(cond, outer_map);
        if ((cond_type == NULL) || (!equals_type(cond_type, bool_type()))){
                return NULL;
        }

        struct parse_tree *body; load_child_at(body, tree, 5);
        struct MAP(string, type) *body_map = new_inner_map();
        update_map(outer_map, body, body_map, parse_tree, MAP(string, type));
        return find_stmts_type(body, outer_map, body_map);
}

const struct type * find_for_type(struct parse_tree *tree, struct OUTER_TYPE_MAP *outer_map){
        verify_type(tree, SYMBOL_STMT);
        struct MAP(string, type) *scope_map = new_inner_map();
        update_map(outer_map, tree, scope_map, parse_tree, MAP(string, type));

        struct parse_tree *init; load_child_at(init, tree, 2);
        const struct type *init_type = find_semistmt_type(init, outer_map, scope_map);
        if (init_type == NULL){
                return NULL;
        }

        struct parse_tree *cond; load_child_at(cond, tree, 4);
        const struct type *cond_type = find_cond_type(cond, outer_map);
        if ((cond_type == NULL) || (!equals_type(cond_type, bool_type()))){
                return NULL;
        }

        struct parse_tree *post; load_child_at(post, tree, 6);
        const struct type *post_type = find_semistmt_type(post, outer_map, scope_map);
        if ((post_type == NULL)){
                return NULL;
        }

        struct parse_tree *body; load_child_at(body, tree, 9);
        const struct type *body_type = find_stmts_type(body, outer_map, scope_map);
        return body_type;
}

const struct type * find_stmt_type(struct parse_tree *tree, struct OUTER_TYPE_MAP *outer_map, struct MAP(string, type) *scope_map){
        verify_type(tree, SYMBOL_STMT);
        switch(tree->children->head->data->data.type){
                case SYMBOL_SEMISTMT:
                        // stmt -> semistmt SEMICOLON
                        return find_semistmt_type(tree->children->head->data, outer_map, scope_map);
                case SYMBOL_IF:
                        return find_if_type(tree, outer_map);
                case SYMBOL_WHILE:
                        return find_while_type(tree, outer_map);
                case SYMBOL_FOR:
                        return find_for_type(tree, outer_map);
                default:
                        return NULL;
        }
}

const struct type * find_stmts_type(struct parse_tree *tree, struct OUTER_TYPE_MAP *outer_map, struct MAP(string, type) *scope_map){
        verify_type(tree, SYMBOL_STMTS);
        struct parse_tree *stmts = tree;

        while (1) {
                // stmts -> stmt stmts
                // stmts -> stmt
                struct parse_tree *stmt = stmts->children->head->data;
                const struct type *stmt_type = find_stmt_type(stmt, outer_map, scope_map);

                if (stmt_type == NULL){
                        return NULL;
                }

                if (stmts->children->len == 2){
                        load_child_at(stmts, stmts, 1);
                } else {
                        return stmt_type;
                }
        }
  
}

struct OUTER_TYPE_MAP * find_types(const struct parse_tree *tree){
        struct OUTER_TYPE_MAP *outer_map = (struct OUTER_TYPE_MAP*) malloc(sizeof (struct OUTER_TYPE_MAP));
        struct MAP(string, type) *inner_map = new_inner_map();
        init_map(outer_map, equals_parse_tree, free_typer_entry, parse_tree, MAP(string, type));
        update_map(outer_map, tree, inner_map, parse_tree, MAP(string, type));

        // program -> optincludes defns END
        struct parse_tree *defns; load_child_at(defns, tree, 1);

        while (1) {
                // defns -> defn defns
                // defns -> defn
                struct parse_tree *defn = defns->children->head->data;

                struct MAP(string, type) *defn_map = new_inner_map();
                update_map(outer_map, defn, defn_map, parse_tree, MAP(string, type));

                const struct type *defn_type = find_defn_type(defn, defn_map);
                struct string *id = (struct string*) malloc(sizeof(struct string));
                id->data = find_defn_name(defn);

                update_map(inner_map, id, defn_type, string, type);

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

                // defn -> type IDENTIFIER LPAREN optparams RPAREN LBRACE stmts RBRACE
                struct parse_tree *stmts; load_child_at(stmts, defn, 6);

                struct MAP(string, type) *stmts_map = new_inner_map(); 
                update_map(outer_map, stmts, stmts_map, parse_tree, MAP(string, type));
                // cast to non-const here because map assumes we can't modify values
                const struct type *stmts_type = find_stmts_type(stmts, outer_map, stmts_map);
                const struct type *ftype = find_symbol_type(defn->children->head->next->data, outer_map);

                if ((stmts_type == NULL) || !equals_type(stmts_type, return_type(ftype))){
                        free_map(outer_map, parse_tree, MAP(string, type));
                        free(outer_map);
                        return NULL;
                }

                if (defns->children->len == 2){
                        load_child_at(defns, defns, 1);
                } else {
                        break;
                }
        } 

        for (struct parse_tree_string_type_map_map_entry_list_node *node = outer_map->list->head; node != NULL; node = node->next) {
                struct OUTER_TYPE_MAP_ENTRY *entry = node->data;
                printf("%s: %d\n", symbol_name(entry->key->data.type), entry->value->list->len);
        }

        return outer_map;
}

void get_variables_recursive(const struct parse_tree *tree, struct LIST(string) *list, struct OUTER_TYPE_MAP *symbols){
        struct MAP(string, type) *inner_map; query_map(symbols, tree, inner_map, parse_tree, MAP(string, type))

        for (struct string_type_map_entry_list_node *node = inner_map->list->head; node != NULL; node = node->next){
                append_list(list, node->data, string);
        }

        for (struct LIST_NODE(parse_tree) *node = tree->children->head; node != NULL; node = node->next){
                get_variables_recursive(node->data, list, symbols);
        }
}

struct LIST(string) get_local_variables(const struct parse_tree *tree, struct OUTER_TYPE_MAP *symbols){
        // defn -> type IDENTIFIER LPAREN optparams RPAREN LBRACE stmts RBRACE
        const struct parse_tree *stmts; load_child_at(stmts, tree, 6);
        struct LIST(string) list;
        init_list((&list))
        get_variables_recursive(stmts, &list, symbols);
        return list;
}

struct LIST(string) get_parameters(const struct parse_tree *tree, struct OUTER_TYPE_MAP *symbols){
        // defn -> type IDENTIFIER LPAREN optparams RPAREN LBRACE stmts RBRACE
        struct MAP(string, type) *inner_map; query_map(symbols, tree, inner_map, parse_tree, MAP(string, type))
        struct LIST(string) list;
        init_list((&list))
        for (struct string_type_map_entry_list_node *node = inner_map->list->head; node != NULL; node = node->next){
                append_list((&list), node->data, string);
        }
        return list;
}