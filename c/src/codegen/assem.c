#include "codegen/assem.h"

#include "assert.h"
#include "stdarg.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"


char *mov(enum reg dest, enum reg src){
        char *instr = (char*) malloc(13 * sizeof(char));
        strcpy(instr, "mov ");
        strcat(instr, reg_to_string(dest));
        strcat(instr, ", ");
        strcat(instr, reg_to_string(src));
        return instr;
}

char *movi(enum reg dest, long long imm){
        assert((-65537 <= imm) && (imm <= 65535));
        char *instr = (char*) malloc(17 * sizeof(char));
        strcpy(instr, "mov ");
        strcat(instr, reg_to_string(dest));
        strcat(instr, ", #");
        char imm_str[7];
        sprintf(imm_str, "%d", imm);
        strcat(instr, imm_str);
        return instr;
}

char *push(enum reg reg){
        char *instr = (char*) malloc(21 * sizeof(char));
        strcpy(instr, "str ");
        strcat(instr, reg_to_string(reg));
        strcat(instr, ", [sp, #-16]!");
        return instr;
}

char *push_pair(enum reg reg1, enum reg reg2){
        char *instr = (char*) malloc(26 * sizeof(char));
        strcpy(instr, "stp ");
        strcat(instr, reg_to_string(reg1));
        strcat(instr, ", ");
        strcat(instr, reg_to_string(reg2));
        strcat(instr, ", [sp, #-16]!");
        return instr;
}

char *pop(enum reg reg){
        char *instr = (char*) malloc(19 * sizeof(char));
        strcpy(instr, "ldr ");
        strcat(instr, reg_to_string(reg));
        strcat(instr, ", [sp], #16");
        return instr;  
}

char *pop_pair(enum reg reg1, enum reg reg2){
        char *instr = (char*) malloc(24 * sizeof(char));
        strcpy(instr, "ldp ");
        strcat(instr, reg_to_string(reg1));
        strcat(instr, ", ");
        strcat(instr, reg_to_string(reg2));
        strcat(instr, ", [sp], #16");
        return instr;
}

char *add(enum reg dest, enum reg src1, enum reg src2){
        char *instr = (char*) malloc(18 * sizeof(char));
        strcpy(instr, "add ");
        strcat(instr, reg_to_string(dest));
        strcat(instr, ", ");
        strcat(instr, reg_to_string(src1));
        strcat(instr, ", ");
        strcat(instr, reg_to_string(src2));
        return instr;
}

char *concat(size_t count, ...){
        va_list args;
        va_start(args, count);
        char *res = NULL;
        size_t len = 1; // for terminating with '\0'

        for (size_t i = 0; i < count; ++i){
                char *cur = va_arg(args, char*);
                size_t curlen = strlen(cur);
                len += curlen;
                if (cur[curlen - 1] != '\n'){
                        ++len;
                }

                char *temp = (char*) malloc(len * sizeof(char));        
                if (res == NULL){
                        strcpy(temp, cur);
                } else {
                        strcpy(temp, res);
                        strcat(temp, cur);
                }
                if (cur[curlen - 1] != '\n'){
                        strcat(temp, "\n");
                }

                if (res != NULL){
                        free(res);
                }
                res = temp;
                free(cur);
        }

        return res;
}

// char *concat(char *line1, char *line2){
//         char *res = (char*) malloc((strlen(line1) + strlen(line2) + 2) * sizeof(char));
//         strcpy(res, line1);
//         strcat(res, "\n");
//         strcat(res, line2);
//         free(line1);
//         free(line2);
//         return res;
// }
