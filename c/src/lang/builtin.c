#include "lang/builtin.h"

#include <string.h>

#include "codegen/assem.h"

void free_builtin(const struct builtin *b){
        free((void*) b);
}

char *evaluate_print_num(char **args){
        char *bl = (char*) malloc(13 * sizeof(char));
        strcpy(bl, "bl print_num");

        return concat(5,
                args[0],
                mov(REG_1, REG_ARITH_RESULT),
                push(REG_LR),
                bl,
                pop(REG_LR)
        );
}

char *evaluate_print_line(char **args){
        char *bl = (char*) malloc(14 * sizeof(char));
        strcpy(bl, "bl print_line");
        return concat(3,
                push(REG_LR),
                bl,
                pop(REG_LR)
        );
}

struct builtin* new_builtin(char *name, const struct type *type, char *(*evaluate)(char **args)){
        struct builtin *ptr = (struct builtin*) malloc(sizeof(struct builtin));
        ptr->name = name;
        ptr->type = type;
        ptr->evaluate = evaluate;
        return ptr;
}

void append_io_builtins(struct LIST(builtin)* list){
        const struct type *print_ret = void_type();
        const struct type *params[MAX_PARAM_COUNT];
        params[0] = int_type();
        struct builtin *print_num = new_builtin("print", function_type(print_ret, params, 1), evaluate_print_num);
        append_list(list, print_num, builtin);

        params[0] = NULL;
        struct builtin *print_line = new_builtin("println", function_type(print_ret, params, 0), evaluate_print_line);
        append_list(list, print_line, builtin);
}

struct LIST(builtin) *get_builtins(struct LIST(string) modules){
        struct LIST(builtin)* list = (struct LIST(builtin)*) malloc(sizeof(struct LIST(builtin)));
        init_list(list);

        for (struct LIST_NODE(string) *module_node = modules.head; module_node != NULL; module_node = module_node->next){
                if (strcmp(module_node->data->data, "poppyio") == 0){
                        append_io_builtins(list);
                }
        }

        return list;
}

char *evaluate_builtin(const struct builtin *builtin, char **args){
        return builtin->evaluate(args);
}

void free_builtins(const struct LIST(builtin) *list){
        if (list != NULL){
                free_list(list, free_builtin, builtin);
        }
}
