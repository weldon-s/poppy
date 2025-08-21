#include "lang/builtin.h"

#include "codegen/assem.h"

void free_builtin(const struct builtin *b){
        free((void*) b);
}

struct LIST(builtin) *list = NULL;

struct builtin* new_builtin(char *name, const struct type *type, char *body){
        struct builtin *ptr = (struct builtin*) malloc(sizeof(struct builtin));
        ptr->name = name;
        ptr->type = type;
        ptr->body = body;
        return ptr;
}

struct LIST(builtin) *get_builtins(){
        if (list != NULL){
                return list;
        }

        list = (struct LIST(builtin)*) malloc(sizeof(struct LIST(builtin)));
        init_list(list);
        const struct type *print_ret = void_type();
        const struct type *params[MAX_PARAM_COUNT];
        params[0] = int_type();


        const struct builtin *print = new_builtin("print", function_type(print_ret, params, 1), 
                "mov x1, x10\n"
                "str lr, [sp, #-16]!\n"
                "bl print_num\n"
                "ldr lr, [sp], #16"
        );

        append_list(list, (struct builtin*) print, builtin);
        return list;
}

void free_builtins(){
        if (list != NULL){
                free_list(list, free_builtin, builtin);
                free(list);
        }
}