#include "token.h"

Token::Token(std::string value, TokenType type) : _value{value}, _type{type} {}

const std::string& Token::value() const {
    return _value;
}

TokenType Token::type() const {
    return _type;
}