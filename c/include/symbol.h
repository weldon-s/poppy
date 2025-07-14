#ifndef SYMBOL_H
#define SYMBOL_H
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
        SYMBOL_END         // end of input
};

#endif
