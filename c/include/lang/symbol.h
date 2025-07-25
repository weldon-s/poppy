#ifndef SYMBOL_H
#define SYMBOL_H

#include <stdbool.h>

enum symbol {
        SYMBOL_NULL,
        // terminal symbols
        SYMBOL_LPAREN,     // (
        SYMBOL_RPAREN,     // )
        SYMBOL_LBRACE,     // {
        SYMBOL_RBRACE,     // }
        SYMBOL_INC,        // ++
        SYMBOL_DEC,        // --
        SYMBOL_AND,        // &&
        SYMBOL_OR,         // ||
        SYMBOL_NOT,        // !
        SYMBOL_PLUS,       // +
        SYMBOL_MINUS,      // -
        SYMBOL_TIMES,      // *
        SYMBOL_DIVIDE,     // /
        SYMBOL_MOD,        // %
        SYMBOL_LT,         // <
        SYMBOL_GT,         // >
        SYMBOL_LE,         // <=
        SYMBOL_GE,         // >=
        SYMBOL_EQ,         // ==
        SYMBOL_NE,         // !=
        SYMBOL_ASSIGN,     // =
        SYMBOL_COMMA,      // ,
        SYMBOL_SEMICOLON,  // ;
        SYMBOL_ELSE,
        SYMBOL_FOR,
        SYMBOL_HOP,
        SYMBOL_IF,
        SYMBOL_INT,
        SYMBOL_LET,
        SYMBOL_MUNCH,
        SYMBOL_WHILE,
        SYMBOL_IDENTIFIER,
        SYMBOL_CONSTANT,
        SYMBOL_END,         // end of input
        // non-terminal symbols
        SYMBOL_PROGRAM,
        SYMBOL_OPTINCLUDES,
        SYMBOL_INCLUDES,
        SYMBOL_INCLUDE,
        SYMBOL_DEFNS,
        SYMBOL_DEFN,
        SYMBOL_TYPE,
        SYMBOL_OPTPARAMS,
        SYMBOL_STMTS,
        SYMBOL_PARAMS,
        SYMBOL_PARAM,
        SYMBOL_STMT,
        SYMBOL_SEMISTMT,
        SYMBOL_EXPR,
        SYMBOL_COND,
        SYMBOL_ANDCOND,
        SYMBOL_ORCOND,
        SYMBOL_UNCOND,
        SYMBOL_OPTELSE,
        SYMBOL_ADDEXPR,
        SYMBOL_MULTEXPR,
        SYMBOL_UNEXPR,
        SYMBOL_OPTARGS,
        SYMBOL_ARGS,
        SYMBOL_VARDEC,
        SYMBOL_VARASST,
        SYMBOL_RET,
        SYMBOL_COUNT
};

static bool is_terminal(enum symbol s){
        return (s > SYMBOL_NULL) && (s <= SYMBOL_END);
}

static char * symbol_name(enum symbol s){
        switch(s) {
        case SYMBOL_NULL: return "null";
        case SYMBOL_LPAREN: return "(";
        case SYMBOL_RPAREN: return ")";
        case SYMBOL_LBRACE: return "{";
        case SYMBOL_RBRACE: return "}";
        case SYMBOL_INC: return "++";
        case SYMBOL_DEC: return "--";
        case SYMBOL_AND: return "&&";
        case SYMBOL_OR: return "||";
        case SYMBOL_NOT: return "!";
        case SYMBOL_PLUS: return "+";
        case SYMBOL_MINUS: return "-";
        case SYMBOL_TIMES: return "*";
        case SYMBOL_DIVIDE: return "/";
        case SYMBOL_MOD: return "%";
        case SYMBOL_LT: return "<";
        case SYMBOL_GT: return ">";
        case SYMBOL_LE: return "<=";
        case SYMBOL_GE: return ">=";
        case SYMBOL_EQ: return "==";
        case SYMBOL_NE: return "!=";
        case SYMBOL_ASSIGN: return "=";
        case SYMBOL_COMMA: return ",";
        case SYMBOL_SEMICOLON: return ";";
        case SYMBOL_ELSE: return "else";
        case SYMBOL_FOR: return "for";
        case SYMBOL_HOP: return "hop";
        case SYMBOL_IF: return "if";
        case SYMBOL_INT: return "int";
        case SYMBOL_LET: return "let";
        case SYMBOL_MUNCH: return "munch";
        case SYMBOL_WHILE: return "while";
        case SYMBOL_IDENTIFIER: return "ID";
        case SYMBOL_CONSTANT: return "CONST";
        case SYMBOL_END: return "END";
        case SYMBOL_PROGRAM: return "PROGRAM";
        case SYMBOL_OPTINCLUDES: return "OPTINCLUDES";
        case SYMBOL_INCLUDES: return "INCLUDES";
        case SYMBOL_INCLUDE: return "INCLUDE";
        case SYMBOL_DEFNS: return "DEFNS";
        case SYMBOL_DEFN: return "DEFN";
        case SYMBOL_TYPE: return "TYPE";
        case SYMBOL_OPTPARAMS: return "OPTPARAMS";
        case SYMBOL_STMTS: return "STMTS";
        case SYMBOL_PARAMS: return "PARAMS";
        case SYMBOL_PARAM: return "PARAM";
        case SYMBOL_STMT: return "STMT";
        case SYMBOL_SEMISTMT: return "SEMISTMT";
        case SYMBOL_EXPR: return "EXPR";
        case SYMBOL_COND: return "COND";
        case SYMBOL_ANDCOND: return "ANDCOND";
        case SYMBOL_ORCOND: return "ORCOND";
        case SYMBOL_UNCOND: return "UNCOND";
        case SYMBOL_OPTELSE: return "OPTELSE";
        case SYMBOL_ADDEXPR: return "ADDEXPR";
        case SYMBOL_MULTEXPR: return "MULTEXPR";
        case SYMBOL_UNEXPR: return "UNEXPR";
        case SYMBOL_OPTARGS: return "OPTARGS";
        case SYMBOL_ARGS: return "ARGS";
        case SYMBOL_VARDEC: return "VARDEC";
        case SYMBOL_VARASST: return "VARASST";
        case SYMBOL_RET: return "RET";
        case SYMBOL_COUNT: return "COUNT";
        }
}

#endif
