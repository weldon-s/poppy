#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>

enum class TokenType;

class Token {
    const std::string _value;
    const TokenType _type;

   public:
    Token(std::string value, TokenType type);
    const std::string& value() const;
    TokenType type() const;
};

enum class TokenType {
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
    END         // end of input
};

#endif