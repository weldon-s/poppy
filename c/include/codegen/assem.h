#ifndef ASSEM_H
#define ASSEM_H

#include <stddef.h>
#include "codegen/register.h"

char *mov(enum reg dest, enum reg src);
char *movi(enum reg dest, long long imm);
char *push(enum reg reg);
char *push_pair(enum reg reg1, enum reg reg2);
char *pop(enum reg reg);
char *pop_pair(enum reg reg1, enum reg reg2);
char *add(enum reg dest, enum reg src1, enum reg src2);
char *concat(size_t count, ...);
#endif
