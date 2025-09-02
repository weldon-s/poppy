#include "lang/type.h"

#include <string.h>

#include "data/list.h"

#define INT_CHAR 'i'
#define VOID_CHAR 'v'
#define BOOL_CHAR 'b'
#define CHAR_CHAR 'c'

DEFINE_LIST(type);

struct LIST(type) types;
bool initialized = false;

struct type *int_ptr = NULL;
struct type *void_ptr = NULL;
struct type *bool_ptr = NULL;
struct type *char_ptr = NULL;

void add_type(struct type *new) {
        if (!initialized) {
                init_list((&types));
                initialized = true;
        }

        append_list((&types), new, type);
}

struct type* simple_type(char repr, bool assignable, bool returnable){
        struct type* new = (struct type*) malloc(sizeof(struct type));
        new->repr[0] = repr;
        new->repr[1] = 0; 
        new->assignable = assignable;
        new->returnable = returnable;
        add_type(new);
        return new;
}

const struct type* const int_type(){
        if (int_ptr == NULL){
                int_ptr = simple_type(INT_CHAR, true, true);
        }
        return int_ptr;
}

const struct type* const bool_type(){
        if (bool_ptr == NULL){
                bool_ptr = simple_type(BOOL_CHAR, true, true);
        }
        return bool_ptr;
}

const struct type* const void_type(){
        if (void_ptr == NULL){
                void_ptr = simple_type(VOID_CHAR, false, true);
        }
        return void_ptr;
}

const struct type* const char_type(){
        if (char_ptr == NULL){
                char_ptr = simple_type(CHAR_CHAR, true, true);
        }
        return char_ptr;
}

const struct type* const function_type(const struct type *ret, const struct type *params[MAX_PARAM_COUNT], size_t params_len){
        if (!ret->returnable){
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

const struct type* const return_type(const struct type *type){
        switch (type->repr[0]){
                case INT_CHAR:
                        return int_type();
                case CHAR_CHAR:
                        return char_type();
                case VOID_CHAR:
                        return void_type();
                case BOOL_CHAR:
                        return bool_type();
                default:
                        return NULL;
        }
}

bool equals_type(const struct type *t1, const struct type *t2){
        return strcmp(t1->repr, t2->repr) == 0;
}

bool equals_arg_types(const struct type *args[MAX_PARAM_COUNT], size_t args_len, const struct type *type){
        size_t params_len = strlen(type->repr) - 2;
        if (params_len != args_len){
                return false;
        }

        for (size_t i = 0; i < args_len; ++i){
                const struct type *param_type;
                switch (type->repr[i + 2]){
                        case INT_CHAR:
                                param_type = int_type();
                                break;
                        case CHAR_CHAR:
                                param_type = char_type();
                                break;
                        case BOOL_CHAR:
                                param_type = bool_type();
                                break;
                        default:
                                return false;
                }

                if (!equals_type(param_type, args[i])){
                        return false;
                }
        }

        return true;
}

void free_types(){
        free_list((&types), free, type);
        int_ptr = NULL;
        bool_ptr = NULL;
        void_ptr = NULL;
        initialized = false;
}
