#include "lexer.h"

#include <cctype>
#include <regex>
#include <stdexcept>

namespace lang {
Lexer::Lexer(std::string source) : source{source} {}

std::vector<Token> Lexer::scan() {
    std::vector<Token> tokens;
    int position = 0;

    while (true) {
        Token token = next_token(&position);
        tokens.push_back(token);

        if (token.type() == Symbol::END) {
            break;
        }
    }

    return tokens;
}

Token Lexer::next_token(int* position) {
    if (*position >= source.size()) {
        return Token("", Symbol::END);
    }

    bool any_skipped = true;

    while (any_skipped) {
        any_skipped = false;

        // skip whitespace
        while (std::isspace(static_cast<unsigned char>(source[*position]))) {
            ++*position;
            any_skipped = true;
        }

        // skip comment
        if (*position + 1 < source.size() && source[*position] == '!' && source[*position + 1] == '!') {
            *position += 2;
            while (*position < source.size() && source[*position] != '\n') {
                ++*position;
            }

            any_skipped = true;
        }

        // skip multiline comment

        if (*position + 1 < source.size() && source[*position] == '!' && source[*position + 1] == '-') {
            *position += 2;
            while (*position + 1 < source.size() && !(source[*position] == '-' && source[*position + 1] == '!')) {
                ++*position;
            }

            if (*position + 1 >= source.size()) {
                throw std::runtime_error("unterminated comment");
            }

            *position += 2;
            any_skipped = true;
        }
    }

    // check for end of input after skipping whitespace
    if (*position >= source.size()) {
        return Token("", Symbol::END);
    }

    // check for single character tokens that aren't prefixes of longer tokens
    switch (source[*position]) {
        case '(':
            ++*position;
            return Token("(", Symbol::LPAREN);
        case ')':
            ++*position;
            return Token(")", Symbol::RPAREN);
        case '{':
            ++*position;
            return Token("{", Symbol::LBRACE);
        case '}':
            ++*position;
            return Token("}", Symbol::RBRACE);
        case '*':
            ++*position;
            return Token("*", Symbol::TIMES);
        case '/':
            ++*position;
            return Token("/", Symbol::DIVIDE);
        case '%':
            ++*position;
            return Token("%", Symbol::MOD);
        case ',':
            ++*position;
            return Token(",", Symbol::COMMA);
        case ';':
            ++*position;
            return Token(";", Symbol::SEMICOLON);

        // check for single character tokens that are prefixes of longer tokens
        case '+':
            if (*position + 1 < source.size() && source[*position + 1] == '+') {
                *position += 2;
                return Token("++", Symbol::INC);
            }

            ++*position;
            return Token("+", Symbol::PLUS);

        case '-':

            if (*position + 1 < source.size() && source[*position + 1] == '-') {
                *position += 2;
                return Token("--", Symbol::DEC);
            }

            ++*position;
            return Token("-", Symbol::MINUS);

        case '<':
            if (*position + 1 < source.size() && source[*position + 1] == '=') {
                *position += 2;
                return Token("<=", Symbol::LE);
            }

            ++*position;
            return Token("<", Symbol::LT);

        case '>':
            if (*position + 1 < source.size() && source[*position + 1] == '=') {
                *position += 2;
                return Token(">=", Symbol::GE);
            }

            ++*position;
            return Token(">", Symbol::GT);

        case '=':
            if (*position + 1 < source.size() && source[*position + 1] == '=') {
                *position += 2;
                return Token("==", Symbol::EQ);
            }

            ++*position;
            return Token("=", Symbol::ASSIGN);

        case '!':
            if (*position + 1 < source.size() && source[*position + 1] == '=') {
                *position += 2;
                return Token("!=", Symbol::NE);
            }

            ++*position;
            return Token("!", Symbol::NOT);

            // check for multi-character tokens with only one value

        case '&':
            if (*position + 1 < source.size() && source[*position + 1] == '&') {
                *position += 2;
                return Token("&&", Symbol::AND);
            }

            throw std::runtime_error("unexpected character &");

        case '|':
            if (*position + 1 < source.size() && source[*position + 1] == '|') {
                *position += 2;
                return Token("||", Symbol::OR);
            }

            throw std::runtime_error("unexpected character |");

        case '"':
            // scan a string literal
            {
                std::string value;
                ++*position;
                while (*position < source.size() && source[*position] != '"') {
                    value += source[*position];
                    ++*position;
                }

                if (*position >= source.size()) {
                    throw std::runtime_error("unterminated string literal");
                }

                ++*position;
                return Token(value, Symbol::STRINGLIT);
            }

        case '\'':
            // scan a character literal
            {
                if (*position + 2 >= source.size() || source[*position + 2] != '\'') {
                    throw std::runtime_error("invalid character literal");
                }

                Token result{{source[*position + 1]}, Symbol::CHARLIT};
                *position += 3;
                return result;
            }

        case '0':
            ++*position;
            return Token("0", Symbol::CONSTANT);

        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            // scan a constant
            {
                std::string value;
                while (*position < source.size() && std::isdigit(static_cast<unsigned char>(source[*position]))) {
                    value += source[*position];
                    ++*position;
                }

                return Token(value, Symbol::CONSTANT);
            }

        default:
            // now, find next whitespace
            {
                const std::string identifier_start{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_"};
                const std::string identifier_middle{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_"};

                if (identifier_start.find(source[*position]) == std::string::npos) {
                    throw std::runtime_error("invalid character " + source[*position]);
                }

                std::string value{source[*position]};

                ++*position;

                while (*position < source.size() && identifier_middle.find(source[*position]) != std::string::npos) {
                    value += source[*position];
                    ++*position;
                }

                if (value == "char") {
                    return Token(value, Symbol::CHAR);
                }

                if (value == "else") {
                    return Token(value, Symbol::ELSE);
                }

                if (value == "for") {
                    return Token(value, Symbol::FOR);
                }

                if (value == "hop") {
                    return Token(value, Symbol::HOP);
                }

                if (value == "if") {
                    return Token(value, Symbol::IF);
                }

                if (value == "int") {
                    return Token(value, Symbol::INT);
                }

                if (value == "string") {
                    return Token(value, Symbol::STRING);
                }

                if (value == "while") {
                    return Token(value, Symbol::WHILE);
                }

                return Token(value, Symbol::IDENTIFIER);
            }
    }
}
}  // namespace lang
