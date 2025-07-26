#include "lang/type.h"

#include <string.h>

#include "data/list.h"

DEFINE_LIST(type);

struct LIST(type) types;
bool initialized = false;

struct type *int_ptr = NULL;
struct type *void_ptr = NULL;
struct type *bool_ptr = NULL;

void add_type(struct type *new) {
        if (!initialized) {
                init_list((&types));
                initialized = true;
        }

        append_list((&types), new, type);
}

struct type* simple_type(char *repr, bool assignable){
        struct type* new = (struct type*) malloc(sizeof(struct type));
        strcpy(new->repr, repr);
        new->assignable = assignable;
        add_type(new);
        return new;
}

const struct type* const int_type(){
        if (int_ptr == NULL){
                int_ptr = simple_type("i", true);
        }
        return int_ptr;
}

const struct type* const bool_type(){
        if (bool_ptr == NULL){
                bool_ptr = simple_type("b", false);
        }
        return bool_ptr;
}

const struct type* const void_type(){
        if (void_ptr == NULL){
                void_ptr = simple_type("v", false);
        }
        return void_ptr;
}

const struct type* const function_type(const struct type *ret, const struct type *params[MAX_PARAM_COUNT], size_t params_len){
        if (!ret->assignable){
                return NULL;
        }

        struct type *new = (struct type*) malloc(sizeof(struct type));
        new->assignable = false;
        strcpy(new->repr, ret->repr);
        strcat(new->repr, " ");

        for (size_t i = 0; i < params_len; ++i){
                if (!params[i]->assignable){
                        free(new);
                        return NULL;
                }
                strcat(new->repr, params[i]->repr);
        }

        add_type(new);
        return new;
}

void free_types(){
        free_list((&types), free, type);
        int_ptr = NULL;
        bool_ptr = NULL;
        void_ptr = NULL;
        initialized = false;
}
