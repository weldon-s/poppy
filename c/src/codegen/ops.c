#include "codegen/ops.h"

#include "codegen/assem.h"

char *binop(char *op1, char *op2, char *operation){
        return concat(5,
                op1,
                push(REG_ARITH_RESULT),
                op2,
                pop(REG_SCRATCH),
                operation
        );
}

char *sum(char *op1, char *op2){
        return binop(op1, op2, add(REG_ARITH_RESULT, REG_SCRATCH, REG_ARITH_RESULT));
}

char *subtract(char *op1, char *op2){
        return binop(op1, op2, sub(REG_ARITH_RESULT, REG_SCRATCH, REG_ARITH_RESULT));
}

char *multiply(char *op1, char *op2){
        return binop(op1, op2, mul(REG_ARITH_RESULT, REG_SCRATCH, REG_ARITH_RESULT));
}

char *divide(char *op1, char *op2){
        return binop(op1, op2, sdiv(REG_ARITH_RESULT, REG_SCRATCH, REG_ARITH_RESULT));
}

char *modulo(char *op1, char *op2){
        // initially, x9 has original number (n), x10 has divisor (d)
        // x8 <- n / d
        // x10 <- n - d * (n / d)
        return binop(op1, op2, concat(2,
             sdiv(REG_8, REG_SCRATCH, REG_ARITH_RESULT),
             msub(REG_ARITH_RESULT, REG_ARITH_RESULT, REG_8, REG_SCRATCH)
        ));
}

char *eq(char *op1, char *op2){
        return binop(op1, op2, concat(2,
                cmp(REG_SCRATCH, REG_ARITH_RESULT),
                cset(REG_ARITH_RESULT, "eq")
        ));
}

char *ne(char *op1, char *op2){
        return binop(op1, op2, concat(2,
                cmp(REG_SCRATCH, REG_ARITH_RESULT),
                cset(REG_ARITH_RESULT, "ne")
        ));
}

char *lt(char *op1, char *op2){
        return binop(op1, op2, concat(2,
                cmp(REG_SCRATCH, REG_ARITH_RESULT),
                cset(REG_ARITH_RESULT, "lt")
        ));
}

char *gt(char *op1, char *op2){
        return binop(op1, op2, concat(2,
                cmp(REG_SCRATCH, REG_ARITH_RESULT),
                cset(REG_ARITH_RESULT, "gt")
        ));
}

char *le(char *op1, char *op2){
        return binop(op1, op2, concat(2,
                cmp(REG_SCRATCH, REG_ARITH_RESULT),
                cset(REG_ARITH_RESULT, "le")
        ));
}

char *ge(char *op1, char *op2){
        return binop(op1, op2, concat(2,
                cmp(REG_SCRATCH, REG_ARITH_RESULT),
                cset(REG_ARITH_RESULT, "ge")
        ));
}

char *cnjtn(char *op1, char *op2){
        return concat(3, comment("and start"), binop(op1, op2, and(REG_ARITH_RESULT, REG_SCRATCH, REG_ARITH_RESULT)), comment("and end"));  
}

char *dsjtn(char *op1, char *op2){
        return concat(3, comment("or start"), binop(op1, op2, orr(REG_ARITH_RESULT, REG_SCRATCH, REG_ARITH_RESULT)), comment("or end"));  

        return binop(op1, op2, orr(REG_ARITH_RESULT, REG_SCRATCH, REG_ARITH_RESULT));  
}

char *ngtn(char *op){
        return concat(2, op, not(REG_ARITH_RESULT, REG_ARITH_RESULT));
}
