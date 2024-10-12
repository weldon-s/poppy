#include "token.h"

Token::Token(std::string value, Symbol type) : _value{value}, _type{type} {}

const std::string& Token::value() const {
    return _value;
}

Symbol Token::type() const {
    return _type;
}