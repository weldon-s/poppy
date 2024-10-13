#ifndef _PARSER_H_
#define _PARSER_H_

#include "grammar.h"
namespace lang {
class Token;

class Parser {
    const Grammar* grammar;

   public:
    Parser(const Grammar* grammar);
    bool parse(const std::vector<Token>& tokens);
};
}  // namespace lang
#endif