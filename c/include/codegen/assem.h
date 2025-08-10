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
char *sub(enum reg dest, enum reg src1, enum reg src2);
char *mul(enum reg dest, enum reg src1, enum reg src2);
char *sdiv(enum reg dest, enum reg src1, enum reg src2);
char *msub(enum reg dest, enum reg mplcand, enum reg mplier, enum reg mend);
char *cmp(enum reg reg1, enum reg reg2);
char *cset(enum reg reg, char cond[3]);
char *and(enum reg dest, enum reg src1, enum reg src2);
char *orr(enum reg dest, enum reg src1, enum reg src2);
char *not(enum reg dest, enum reg src);
char *cmpi(enum reg reg, long long imm);
char *ret();
char *concat(size_t count, ...);
#endif
