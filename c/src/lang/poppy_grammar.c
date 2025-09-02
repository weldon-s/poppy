#include "lang/poppy_grammar.h"

#include <stdlib.h>
#include <string.h>

#define RULE_COUNT 70
#define COMMA ,
#define populate(lh_symbol, rh_symbols, ctr, grmr)                               \
        do {                                                                     \
                grmr->rules[ctr].lhs = lh_symbol;                                \
                enum symbol rhs[] = rh_symbols;                                  \
                grmr->rules[ctr].rhs = (enum symbol*) malloc(sizeof(rhs));       \
                grmr->rules[ctr].rhs_len = sizeof(rhs) / sizeof(enum symbol);    \
                memcpy(grmr->rules[ctr].rhs, rhs, sizeof(rhs));                  \
        } while (0)

struct grammar * poppy_grammar = NULL;

const struct grammar * const get_poppy_grammar(){
        if (poppy_grammar != NULL){
                return poppy_grammar;
        }

        poppy_grammar = (struct grammar*) malloc(sizeof(struct grammar));
        poppy_grammar->start = SYMBOL_PROGRAM;
        poppy_grammar->rules_len = RULE_COUNT;

        for (size_t i = 0; i < SYMBOL_COUNT; ++i){
                poppy_grammar->nullable[i] = false;
        }

        poppy_grammar->rules = (struct rule*) malloc(RULE_COUNT * sizeof(struct rule));

        int i = 0;
        populate(SYMBOL_PROGRAM, {SYMBOL_OPTINCLUDES COMMA SYMBOL_DEFNS COMMA SYMBOL_END}, i, poppy_grammar); ++i;
        populate(SYMBOL_OPTINCLUDES, {SYMBOL_INCLUDES},i, poppy_grammar); ++i;
        populate(SYMBOL_OPTINCLUDES, {}, i, poppy_grammar); ++i;
        populate(SYMBOL_INCLUDES, {SYMBOL_INCLUDE COMMA SYMBOL_INCLUDES}, i, poppy_grammar); ++i;
        populate(SYMBOL_INCLUDES, {SYMBOL_INCLUDE}, i, poppy_grammar); ++i;
        populate(SYMBOL_INCLUDE, {SYMBOL_MUNCH COMMA SYMBOL_IDENTIFIER}, i, poppy_grammar); ++i;
        populate(SYMBOL_DEFNS, {SYMBOL_DEFN}, i, poppy_grammar); ++i;
        populate(SYMBOL_DEFNS, {SYMBOL_DEFN COMMA SYMBOL_DEFNS}, i, poppy_grammar); ++i;
        populate(SYMBOL_DEFN, {SYMBOL_TYPE COMMA SYMBOL_IDENTIFIER COMMA SYMBOL_LPAREN COMMA SYMBOL_OPTPARAMS COMMA SYMBOL_RPAREN COMMA SYMBOL_LBRACE COMMA SYMBOL_STMTS COMMA SYMBOL_RBRACE}, i, poppy_grammar); ++i;
        populate(SYMBOL_TYPE, {SYMBOL_INT}, i, poppy_grammar); ++i;
        populate(SYMBOL_TYPE, {SYMBOL_VOID}, i, poppy_grammar); ++i;
        populate(SYMBOL_TYPE, {SYMBOL_CHAR}, i, poppy_grammar); ++i;
        populate(SYMBOL_TYPE, {SYMBOL_BOOL}, i, poppy_grammar); ++i;
        populate(SYMBOL_OPTPARAMS, {}, i, poppy_grammar); ++i;
        populate(SYMBOL_OPTPARAMS, {SYMBOL_PARAMS}, i, poppy_grammar); ++i;
        populate(SYMBOL_PARAMS, {SYMBOL_PARAM COMMA SYMBOL_COMMA COMMA SYMBOL_PARAMS}, i, poppy_grammar); ++i;
        populate(SYMBOL_PARAMS, {SYMBOL_PARAM}, i, poppy_grammar); ++i;
        populate(SYMBOL_PARAM, {SYMBOL_TYPE COMMA SYMBOL_IDENTIFIER}, i, poppy_grammar); ++i;
        populate(SYMBOL_STMTS, {SYMBOL_STMT}, i, poppy_grammar); ++i;
        populate(SYMBOL_STMTS, {SYMBOL_STMT COMMA SYMBOL_STMTS}, i, poppy_grammar); ++i;
        populate(SYMBOL_STMT, {SYMBOL_SEMISTMT COMMA SYMBOL_SEMICOLON}, i, poppy_grammar); ++i;
        populate(SYMBOL_SEMISTMT, {SYMBOL_VARDEC}, i, poppy_grammar); ++i;
        populate(SYMBOL_VARDEC, {SYMBOL_LET COMMA SYMBOL_TYPE COMMA SYMBOL_IDENTIFIER COMMA SYMBOL_ASSIGN COMMA SYMBOL_EXPR}, i, poppy_grammar); ++i;
        populate(SYMBOL_VARDEC, {SYMBOL_LET COMMA SYMBOL_TYPE COMMA SYMBOL_IDENTIFIER}, i, poppy_grammar); ++i;
        populate(SYMBOL_SEMISTMT, {SYMBOL_VARASST}, i, poppy_grammar); ++i;
        populate(SYMBOL_VARASST, {SYMBOL_IDENTIFIER COMMA SYMBOL_ASSIGN COMMA SYMBOL_EXPR}, i, poppy_grammar); ++i;
        populate(SYMBOL_SEMISTMT, {SYMBOL_RET}, i, poppy_grammar); ++i;
        populate(SYMBOL_RET, {SYMBOL_HOP COMMA SYMBOL_EXPR}, i, poppy_grammar); ++i;
        populate(SYMBOL_RET, {SYMBOL_HOP}, i, poppy_grammar); ++i;
        populate(SYMBOL_SEMISTMT, {SYMBOL_EXPR}, i, poppy_grammar); ++i;
        populate(SYMBOL_STMT, {SYMBOL_IF COMMA SYMBOL_LPAREN COMMA SYMBOL_EXPR COMMA SYMBOL_RPAREN COMMA SYMBOL_LBRACE COMMA SYMBOL_STMTS COMMA SYMBOL_RBRACE COMMA SYMBOL_OPTELSE }, i, poppy_grammar); ++i;
        populate(SYMBOL_OPTELSE, {}, i, poppy_grammar); ++i;
        populate(SYMBOL_OPTELSE, {SYMBOL_ELSE COMMA SYMBOL_LBRACE COMMA SYMBOL_STMTS COMMA SYMBOL_RBRACE}, i, poppy_grammar); ++i;
        populate(SYMBOL_STMT, {SYMBOL_WHILE COMMA SYMBOL_LPAREN COMMA SYMBOL_EXPR COMMA SYMBOL_RPAREN COMMA SYMBOL_LBRACE COMMA SYMBOL_STMTS COMMA SYMBOL_RBRACE}, i, poppy_grammar); ++i;
        populate(SYMBOL_STMT, {SYMBOL_FOR COMMA SYMBOL_LPAREN COMMA SYMBOL_SEMISTMT COMMA SYMBOL_SEMICOLON COMMA SYMBOL_EXPR COMMA SYMBOL_SEMICOLON COMMA SYMBOL_SEMISTMT COMMA SYMBOL_RPAREN COMMA SYMBOL_LBRACE COMMA SYMBOL_STMTS COMMA SYMBOL_RBRACE}, i, poppy_grammar); ++i;
        populate(SYMBOL_EXPR, {SYMBOL_ANDCOND}, i, poppy_grammar); ++i;
        populate(SYMBOL_ANDCOND, {SYMBOL_ANDCOND COMMA SYMBOL_AND COMMA SYMBOL_ORCOND}, i, poppy_grammar); ++i;
        populate(SYMBOL_ANDCOND, {SYMBOL_ORCOND}, i, poppy_grammar); ++i;
        populate(SYMBOL_ORCOND, {SYMBOL_ORCOND COMMA SYMBOL_OR COMMA SYMBOL_UNCOND}, i, poppy_grammar); ++i;
        populate(SYMBOL_ORCOND, {SYMBOL_UNCOND}, i, poppy_grammar); ++i;
        populate(SYMBOL_UNCOND, {SYMBOL_NOT COMMA SYMBOL_EXPR}, i, poppy_grammar); ++i;
        populate(SYMBOL_UNCOND, {SYMBOL_LPAREN COMMA SYMBOL_EXPR COMMA SYMBOL_RPAREN}, i, poppy_grammar); ++i;
        populate(SYMBOL_UNCOND, {SYMBOL_EXPR COMMA SYMBOL_LT COMMA SYMBOL_EXPR}, i, poppy_grammar); ++i;
        populate(SYMBOL_UNCOND, {SYMBOL_EXPR COMMA SYMBOL_GT COMMA SYMBOL_EXPR}, i, poppy_grammar); ++i;
        populate(SYMBOL_UNCOND, {SYMBOL_EXPR COMMA SYMBOL_LE COMMA SYMBOL_EXPR}, i, poppy_grammar); ++i;
        populate(SYMBOL_UNCOND, {SYMBOL_EXPR COMMA SYMBOL_GE COMMA SYMBOL_EXPR}, i, poppy_grammar); ++i;
        populate(SYMBOL_UNCOND, {SYMBOL_EXPR COMMA SYMBOL_EQ COMMA SYMBOL_EXPR}, i, poppy_grammar); ++i;
        populate(SYMBOL_UNCOND, {SYMBOL_EXPR COMMA SYMBOL_NE COMMA SYMBOL_EXPR}, i, poppy_grammar); ++i;
        populate(SYMBOL_UNCOND, {SYMBOL_TRUE}, i, poppy_grammar); ++i;
        populate(SYMBOL_UNCOND, {SYMBOL_FALSE}, i, poppy_grammar); ++i;
        populate(SYMBOL_EXPR, {SYMBOL_ADDEXPR}, i, poppy_grammar); ++i;
        populate(SYMBOL_ADDEXPR, {SYMBOL_ADDEXPR COMMA SYMBOL_PLUS COMMA SYMBOL_MULTEXPR}, i, poppy_grammar); ++i;
        populate(SYMBOL_ADDEXPR, {SYMBOL_ADDEXPR COMMA SYMBOL_MINUS COMMA SYMBOL_MULTEXPR}, i, poppy_grammar); ++i;
        populate(SYMBOL_ADDEXPR, {SYMBOL_MULTEXPR}, i, poppy_grammar); ++i;
        populate(SYMBOL_MULTEXPR, {SYMBOL_MULTEXPR COMMA SYMBOL_TIMES COMMA SYMBOL_UNEXPR}, i, poppy_grammar); ++i;
        populate(SYMBOL_MULTEXPR, {SYMBOL_MULTEXPR COMMA SYMBOL_DIVIDE COMMA SYMBOL_UNEXPR}, i, poppy_grammar); ++i;
        populate(SYMBOL_MULTEXPR, {SYMBOL_MULTEXPR COMMA SYMBOL_MOD COMMA SYMBOL_UNEXPR}, i, poppy_grammar); ++i;
        populate(SYMBOL_MULTEXPR, {SYMBOL_UNEXPR}, i, poppy_grammar); ++i;
        populate(SYMBOL_UNEXPR, {SYMBOL_MINUS COMMA SYMBOL_UNEXPR}, i, poppy_grammar); ++i;
        populate(SYMBOL_UNEXPR, {SYMBOL_LPAREN COMMA SYMBOL_EXPR COMMA SYMBOL_RPAREN}, i, poppy_grammar); ++i;
        populate(SYMBOL_UNEXPR, {SYMBOL_IDENTIFIER COMMA SYMBOL_LPAREN COMMA SYMBOL_OPTARGS COMMA SYMBOL_RPAREN}, i, poppy_grammar); ++i;
        populate(SYMBOL_OPTARGS, {}, i, poppy_grammar); ++i;
        populate(SYMBOL_OPTARGS, {SYMBOL_ARGS}, i, poppy_grammar); ++i;
        populate(SYMBOL_ARGS, {SYMBOL_EXPR}, i, poppy_grammar); ++i;
        populate(SYMBOL_ARGS, {SYMBOL_EXPR COMMA SYMBOL_COMMA COMMA SYMBOL_ARGS}, i, poppy_grammar); ++i;
        populate(SYMBOL_UNEXPR, {SYMBOL_INC COMMA SYMBOL_IDENTIFIER}, i, poppy_grammar); ++i;
        populate(SYMBOL_UNEXPR, {SYMBOL_DEC COMMA SYMBOL_IDENTIFIER}, i, poppy_grammar); ++i;
        populate(SYMBOL_UNEXPR, {SYMBOL_IDENTIFIER}, i, poppy_grammar); ++i;
        populate(SYMBOL_UNEXPR, {SYMBOL_CONSTANT}, i, poppy_grammar); ++i;
        populate(SYMBOL_UNEXPR, {SYMBOL_SQUOTE COMMA SYMBOL_CHARLIT COMMA SYMBOL_SQUOTE}, i, poppy_grammar); ++i;

        for (size_t i = 0; i < RULE_COUNT; ++i){
                if (poppy_grammar->rules[i].rhs_len == 0){
                        poppy_grammar->nullable[poppy_grammar->rules[i].lhs] = true;
                }
        }

        return poppy_grammar;

}

void free_poppy_grammar(){
        if(poppy_grammar == NULL){
                return;
        }

        for(int i = 0; i < poppy_grammar->rules_len; ++i){
                free(poppy_grammar->rules[i].rhs);
        }

        free(poppy_grammar->rules);
        free(poppy_grammar);
        poppy_grammar = NULL;
}
