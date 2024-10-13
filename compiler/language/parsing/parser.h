#ifndef _PARSER_H_
#define _PARSER_H_

#include <optional>

#include "grammar.h"
#include "parsetree.h"
namespace lang {
class Token;

class Parser {
    const Grammar* grammar;

   public:
    Parser(const Grammar* grammar);
    std::optional<ParseTree> parse(const std::vector<Token>& tokens);
};
}  // namespace lang
#endif