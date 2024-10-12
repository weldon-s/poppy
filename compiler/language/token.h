#ifndef _TOKEN_H_
#define _TOKEN_H_

#include <string>

enum class Symbol;

class Token {
    const std::string _value;
    const Symbol _type;

   public:
    Token(std::string value, Symbol type);
    const std::string& value() const;
    Symbol type() const;
};

#endif