#include "lang/builtin.h"

#include <string.h>

#include "codegen/assem.h"

void free_builtin(const struct builtin *b){
        free((void*) b);
}

// struct LIST(builtin) *list = NULL;

struct builtin* new_builtin(char *name, const struct type *type, char *body){
        struct builtin *ptr = (struct builtin*) malloc(sizeof(struct builtin));
        ptr->name = name;
        ptr->type = type;
        ptr->body = body;
        return ptr;
}

void append_io_builtins(struct LIST(builtin)* list){
        const struct type *print_ret = void_type();
        const struct type *params[MAX_PARAM_COUNT];
        params[0] = int_type();
        struct builtin *print_num = new_builtin("print", function_type(print_ret, params, 1), 
                "mov x1, x10\n"
                "str lr, [sp, #-16]!\n"
                "bl print_num\n"
                "ldr lr, [sp], #16"
        );
        append_list(list, print_num, builtin);

        params[0] = NULL;
        struct builtin *print_line = new_builtin("println", function_type(print_ret, params, 0),
                "str lr, [sp, #-16]!\n"
                "bl print_line\n"
                "ldr lr, [sp], #16"
        );
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

void free_builtins(const struct LIST(builtin) *list){
        if (list != NULL){
                free_list(list, free_builtin, builtin);
        }
}
