#ifndef _SYMBOL_H_
#define _SYMBOL_H_

enum class Symbol {
    // terminal symbols
    LPAREN,     // (
    RPAREN,     // )
    LBRACE,     // {
    RBRACE,     // }
    INC,        // ++
    DEC,        // --
    AND,        // &&
    OR,         // ||
    NOT,        // !
    PLUS,       // +
    MINUS,      // -
    TIMES,      // *
    DIVIDE,     // /
    MOD,        // %
    LT,         // <
    GT,         // >
    LE,         // <=
    GE,         // >=
    EQ,         // ==
    NE,         // !=
    ASSIGN,     // =
    COMMA,      // ,
    SEMICOLON,  // ;
    CHAR,
    ELSE,
    FOR,
    HOP,
    IF,
    INT,
    STRING,
    WHILE,
    IDENTIFIER,
    CONSTANT,
    STRINGLIT,  // string literal
    CHARLIT,    // character literal
    END,        // end of input

    // non-terminal symbols
    PROGRAM,
    DEFNS,
    DEFN,
    TYPE,
    OPTPARAMS,
    STMTS,
    PARAMS,
    PARAM,
    STMT,
    SEMISTMT,
    EXPR,
    COND,
    ANDCOND,
    ORCOND,
    UNCOND,
    OPTELSE,
    ADDEXPR,
    MULTEXPR,
    UNEXPR
};

constexpr bool is_terminal(Symbol s) {
    return s <= Symbol::END;
}

#endif