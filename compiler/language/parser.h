#ifndef _PARSER_H_
#define _PARSER_H_

#include "grammar.h"

class Token;

class Parser {
    const Grammar* grammar;

   public:
    Parser(const Grammar* grammar);
    bool parse(const std::vector<Token>& tokens);
};

#endif