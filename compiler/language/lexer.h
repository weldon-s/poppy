#ifndef _LEXER_H_
#define _LEXER_H_

#include <string>
#include <vector>

#include "language/token.h"

class Lexer {
    const std::string source;

    // using a pointer to position to allow for updating the position
    Token next_token(int* position);

   public:
    Lexer(std::string source);
    std::vector<Token> scan();
};

#endif