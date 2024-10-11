#include "lexer.h"

#include <cctype>
#include <regex>
#include <stdexcept>

Lexer::Lexer(std::string source) : source{source} {}

std::vector<Token> Lexer::scan() {
    std::vector<Token> tokens;
    int position = 0;

    while (true) {
        Token token = next_token(&position);
        tokens.push_back(token);

        if (token.type() == TokenType::END) {
            break;
        }
    }

    return tokens;
}

Token Lexer::next_token(int* position) {
    if (*position >= source.size()) {
        return Token("", TokenType::END);
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
        return Token("", TokenType::END);
    }

    // check for single character tokens that aren't prefixes of longer tokens
    switch (source[*position]) {
        case '(':
            ++*position;
            return Token("(", TokenType::LPAREN);
        case ')':
            ++*position;
            return Token(")", TokenType::RPAREN);
        case '{':
            ++*position;
            return Token("{", TokenType::LBRACE);
        case '}':
            ++*position;
            return Token("}", TokenType::RBRACE);
        case '*':
            ++*position;
            return Token("*", TokenType::TIMES);
        case '/':
            ++*position;
            return Token("/", TokenType::DIVIDE);
        case '%':
            ++*position;
            return Token("%", TokenType::MOD);
        case ',':
            ++*position;
            return Token(",", TokenType::COMMA);
        case ';':
            ++*position;
            return Token(";", TokenType::SEMICOLON);

        // check for single character tokens that are prefixes of longer tokens
        case '+':
            if (*position + 1 < source.size() && source[*position + 1] == '+') {
                *position += 2;
                return Token("++", TokenType::INC);
            }

            ++*position;
            return Token("+", TokenType::PLUS);

        case '-':

            if (*position + 1 < source.size() && source[*position + 1] == '-') {
                *position += 2;
                return Token("--", TokenType::DEC);
            }

            ++*position;
            return Token("-", TokenType::MINUS);

        case '<':
            if (*position + 1 < source.size() && source[*position + 1] == '=') {
                *position += 2;
                return Token("<=", TokenType::LE);
            }

            ++*position;
            return Token("<", TokenType::LT);

        case '>':
            if (*position + 1 < source.size() && source[*position + 1] == '=') {
                *position += 2;
                return Token(">=", TokenType::GE);
            }

            ++*position;
            return Token(">", TokenType::GT);

        case '=':
            if (*position + 1 < source.size() && source[*position + 1] == '=') {
                *position += 2;
                return Token("==", TokenType::EQ);
            }

            ++*position;
            return Token("=", TokenType::ASSIGN);

        case '!':
            if (*position + 1 < source.size() && source[*position + 1] == '=') {
                *position += 2;
                return Token("!=", TokenType::NE);
            }

            ++*position;
            return Token("!", TokenType::NOT);

            // check for multi-character tokens with only one value

        case '&':
            if (*position + 1 < source.size() && source[*position + 1] == '&') {
                *position += 2;
                return Token("&&", TokenType::AND);
            }

            throw std::runtime_error("unexpected character &");

        case '|':
            if (*position + 1 < source.size() && source[*position + 1] == '|') {
                *position += 2;
                return Token("||", TokenType::OR);
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
                return Token(value, TokenType::STRINGLIT);
            }

        case '\'':
            // scan a character literal
            {
                if (*position + 2 >= source.size() || source[*position + 2] != '\'') {
                    throw std::runtime_error("invalid character literal");
                }

                Token result{source[*position + 1] + "", TokenType::CHARLIT};
                *position += 3;
                return result;
            }

        case '0':
            ++*position;
            return Token("0", TokenType::CONSTANT);

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

                return Token(value, TokenType::CONSTANT);
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
                    return Token(value, TokenType::CHAR);
                }

                if (value == "else") {
                    return Token(value, TokenType::ELSE);
                }

                if (value == "for") {
                    return Token(value, TokenType::FOR);
                }

                if (value == "hop") {
                    return Token(value, TokenType::HOP);
                }

                if (value == "if") {
                    return Token(value, TokenType::IF);
                }

                if (value == "int") {
                    return Token(value, TokenType::INT);
                }

                if (value == "string") {
                    return Token(value, TokenType::STRING);
                }

                if (value == "while") {
                    return Token(value, TokenType::WHILE);
                }

                return Token(value, TokenType::IDENTIFIER);
            }
    }
}