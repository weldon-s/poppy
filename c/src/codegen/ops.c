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
