#ifndef BINOPS_H
#define BINOPS_H

char *sum(char *op1, char *op2);
char *subtract(char *op1, char *op2);
char *multiply(char *op1, char *op2);
char *divide(char *op1, char *op2);
char *modulo(char *op1, char *op2);
char *eq(char *op1, char *op2);
char *ne(char *op1, char *op2);
char *lt(char *op1, char *op2);
char *le(char *op1, char *op2);
char *gt(char *op1, char *op2);
char *ge(char *op1, char *op2);
char *and(char *op1, char *op2);
char *or(char *op1, char *op2);
char *not(char *op1);

#endif
