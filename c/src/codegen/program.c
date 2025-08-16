#include "codegen/program.h"

#include <stdlib.h>
#include <string.h>

#include "codegen/assem.h"
#include "codegen/control.h"
#include "codegen/function.h"
#include "codegen/ops.h"
#include "lang/symbol.h"

#define load_child_at(var, tree, n)                                        \
        do {                                                               \
                struct LIST_NODE(parse_tree) *node = tree->children->head; \
                for (int i = 0; i < n; ++i){                               \
                        node = node->next;                                 \
                }                                                          \
                var = node->data;                                          \
        } while (0);                                                       \

DEFINE_MAP(string, function);

const char *head = ".text\n"
                     ".global _start";

const char *tail = "mov x0, #0\n"
                  "mov w8, #93\n"
                  "svc #0\n";

const char *start_label = "_start:";

void free_string_function_entry(const struct MAP_ENTRY(string, function) *entry) {
        free_function(entry->key);
        free_function(entry->value);
        free((void*) entry);
}

char *generate_from_tree(struct parse_tree *tree, struct MAP(string, function) *functions, struct function *within){
        enum symbol symbol = tree->data.type;

        if (symbol == SYMBOL_STMTS){
                char *first_stmt = generate_from_tree(tree->children->head->data, functions, within);
                if (tree->children->len == 1){
                        return first_stmt;
                }

                return concat(2, first_stmt, generate_from_tree(tree->children->head->next->data, functions, within));
        } else if (symbol == SYMBOL_STMT){
                enum symbol child_symbol = tree->children->head->data->data.type;
                if (child_symbol == SYMBOL_SEMISTMT){
                        return generate_from_tree(tree->children->head->data, functions, within);
                } else if (child_symbol == SYMBOL_IF){
                        struct parse_tree *cond; load_child_at(cond, tree, 2);
                        char *cond_code = generate_from_tree(cond, functions, within);
                        struct parse_tree *then; load_child_at(then, tree, 5);
                        char *then_code = generate_from_tree(then, functions, within);
                        struct parse_tree *optelse;  load_child_at(optelse, tree, 7);

                        if ((optelse->children == NULL) || (optelse->children->len == 0)){
                                return if_stmt(cond_code, then_code, NULL);
                        }

                        struct parse_tree *else_stmts; load_child_at(else_stmts, optelse, 2);
                        char *else_code = generate_from_tree(else_stmts, functions, within);
                        return if_stmt(cond_code, then_code, else_code);
                } else if (child_symbol == SYMBOL_WHILE){
                        struct parse_tree *cond; load_child_at(cond, tree, 2);
                        char *cond_code = generate_from_tree(cond, functions, within);
                        struct parse_tree *stmts; load_child_at(stmts, tree, 5);
                        char *stmts_code = generate_from_tree(stmts, functions, within);
                        return while_loop(cond_code, stmts_code);
                } else if (child_symbol == SYMBOL_FOR){
                        struct parse_tree *init; load_child_at(init, tree, 2);
                        char *init_code = generate_from_tree(init, functions, within);
                        struct parse_tree *cond; load_child_at(cond, tree, 4);
                        char *cond_code = generate_from_tree(cond, functions, within);
                        struct parse_tree *post; load_child_at(post, tree, 6);
                        char *post_code = generate_from_tree(post, functions, within);
                        struct parse_tree *body; load_child_at(post, tree, 9);
                        char *body_code = generate_from_tree(body, functions, within);                        
                        return for_loop(init, cond, post, body);                  
                }
        } else if (symbol == SYMBOL_SEMISTMT){
                return generate_from_tree(tree->children->head->data, functions, within);
        } else if (symbol == SYMBOL_VARDEC){
                if (tree->children->len == 5){
                        struct parse_tree *id; load_child_at(id, tree, 2);
                        char *var = id->data.value;
                        struct parse_tree *expr; load_child_at(expr, tree, 4);
                        char *expr_code = generate_from_tree(expr, functions, within);
                        return concat(2, expr_code, write_function_variable(within, var, REG_ARITH_RESULT));
                }
        } else if (symbol == SYMBOL_VARASST){
                struct parse_tree *id; load_child_at(id, tree, 0);
                char *var = id->data.value;
                struct parse_tree *expr; load_child_at(expr, tree, 2);
                char *expr_code = generate_from_tree(expr, functions, within);
                return concat(2, expr_code, write_function_variable(within, var, REG_ARITH_RESULT));                
        } else if (symbol == SYMBOL_RET){
                struct parse_tree *expr; load_child_at(expr, tree, 1);
                char *expr_code = generate_from_tree(expr, functions, within);
                return concat(2, expr_code, ret());
        } else if (symbol == SYMBOL_COND){
                return generate_from_tree(tree->children->head->data, functions, within);
        } else if (symbol == SYMBOL_ANDCOND){
                if (tree->children->len == 1){
                        return generate_from_tree(tree->children->head->data, functions, within);
                }

                char *op1 = generate_from_tree(tree->children->head->data, functions, within);
                char *op2 = generate_from_tree(tree->children->head->next->next->data, functions, within);
                return cnjtn(op1, op2);
        } else if (symbol == SYMBOL_ORCOND){
                if (tree->children->len == 1){
                        return generate_from_tree(tree->children->head->data, functions, within);
                }

                char *op1 = generate_from_tree(tree->children->head->data, functions, within);
                char *op2 = generate_from_tree(tree->children->head->next->next->data, functions, within);
                return dsjtn(op1, op2);                
        } else if (symbol == SYMBOL_UNCOND){
                if (tree->children->len == 2){
                        return ngtn(generate_from_tree(tree->children->head->next->data, functions, within));
                }

                enum symbol op_type = tree->children->head->next->data->data.type;
                if (op_type == SYMBOL_COND){
                        return generate_from_tree(tree->children->head->next->data, functions, within);
                }

                char *op1 = generate_from_tree(tree->children->head->data, functions, within);
                char *op2 = generate_from_tree(tree->children->head->next->next->data, functions, within);
                switch (op_type){
                        case SYMBOL_LT:
                                return lt(op1, op2);
                        case SYMBOL_GT:
                                return gt(op1, op2);
                        case SYMBOL_LE:
                                return le(op1, op2);
                        case SYMBOL_GE:
                                return ge(op1, op2);
                        case SYMBOL_EQ:
                                return eq(op1, op2);
                        case SYMBOL_NE:
                                return ne(op1, op2);
                }
        } else if (symbol == SYMBOL_EXPR){
                return generate_from_tree(tree->children->head->data, functions, within);
        } else if ((symbol == SYMBOL_ADDEXPR) || (symbol == SYMBOL_MULTEXPR)){
                if (tree->children->len == 1){
                        return generate_from_tree(tree->children->head->data, functions, within);
                }

                char *op1 = generate_from_tree(tree->children->head->data, functions, within);
                char *op2 = generate_from_tree(tree->children->head->next->next->data, functions, within);
                enum symbol operand = tree->children->head->next->data->data.type;
                switch(operand){
                        case SYMBOL_PLUS:
                                return sum(op1, op2);
                        case SYMBOL_MINUS:
                                return subtract(op1, op2);
                        case SYMBOL_TIMES:
                                return multiply(op1, op2);
                        case SYMBOL_DIVIDE:
                                return divide(op1, op2);
                        case SYMBOL_MOD:
                                return modulo(op1, op2);
                }

                return subtract(op1, op2);
        } else if (symbol == SYMBOL_UNEXPR){          
                enum symbol first = tree->children->head->data->data.type;
                if (first == SYMBOL_MINUS){
                        return subtract(
                                movi(REG_ARITH_RESULT, 0), 
                                generate_from_tree(tree->children->head->next->data, functions, within)
                        );
                }

                if (first == SYMBOL_INC){
                        char *var = tree->children->head->next->data->data.value;
                        return concat(2,
                                sum(
                                        read_function_variable(within, REG_ARITH_RESULT, var),
                                        movi(REG_ARITH_RESULT, 1)
                                ),
                                write_function_variable(within, var, REG_ARITH_RESULT)
                        );
                }

                if (first == SYMBOL_DEC){
                        char *var = tree->children->head->next->data->data.value;
                        return concat(2,
                                subtract(
                                        read_function_variable(within, REG_ARITH_RESULT, var),
                                        movi(REG_ARITH_RESULT, 1)
                                ),
                                write_function_variable(within, var, REG_ARITH_RESULT)
                        );
                }

                if (first == SYMBOL_LPAREN){
                        return generate_from_tree(tree->children->head->next->data, functions, within);
                }

                if (first == SYMBOL_CONSTANT){
                        long long imm = strtoll(tree->children->head->data->data.value, NULL, 10);
                        return movi(REG_ARITH_RESULT, imm);
                }

                if (first == SYMBOL_IDENTIFIER){
                        char *id = tree->children->head->data->data.value;

                        if (tree->children->len == 1){
                                return read_function_variable(within, REG_ARITH_RESULT, id);
                        }

                        struct string s;
                        s.data = id;
                        struct function *f; query_map(functions, (&s), f, string, function);
                        if (num_params(f) == 0){
                                return call_function(f, NULL);
                        }

                        char **args_code = (char**) malloc(num_params(f) * sizeof(char*));

                        size_t i = 0;
                        struct parse_tree *args = tree->children->head->next->next->next->data->children->head->data;
                        while (1) {
                                // args -> expr COMMA args
                                // args -> expr
                                struct parse_tree *expr = args->children->head->data;
                                args_code[i++] = generate_from_tree(expr, functions, within);

                                if (args->children->len == 3){
                                        load_child_at(args, args, 2);
                                } else {
                                        break;
                                }
                        };
                        char *ret = call_function(f, args_code);
                        free(args_code);
                        return ret;
                }
        }
}

char *generate_code(struct OUTER_TYPE_MAP *type_map, struct parse_tree *tree){
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
                struct string s;
                s.data = defn->children->head->next->data->data.value;;
                struct function *fn; query_map((&functions), (&s), fn, string, function);
                struct parse_tree *stmts; load_child_at(stmts, defn, 6);
                set_body(fn, generate_from_tree(stmts, &functions, fn));

                if (defns->children->len == 2){
                        load_child_at(defns, defns, 1);
                } else {
                        break;
                }
        }

        char *prog = (char*) malloc(21 * sizeof(char));
        strcpy(prog, head);
        
        for (struct string_function_map_entry_list_node *node = functions.list->head; node != NULL; node = node->next){
                if (strcmp(node->data->key->data, "main") == 0){
                        continue;
                }

                struct function *f = node->data->value;
                prog = concat(2, prog, declare_function(f));
        }

        struct string s = {"main"};
        struct function *main; query_map((&functions), (&s), main, string, function);
        char *sl = (char*) malloc(8 * sizeof(char));
        char *t = (char*) malloc(31 * sizeof(char));
        strcpy(sl, start_label);
        strcpy(t, tail);
        prog = concat(4, prog, sl, declare_function(main), t);
}
