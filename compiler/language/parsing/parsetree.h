#ifndef _PARSETREE_H_
#define _PARSETREE_H_

#include <vector>

#include "language/lexing/token.h"

namespace lang {
enum class Symbol;

class ParseTree {
    const Token _data;
    std::vector<ParseTree> _children;

   public:
    ParseTree(Token token);                                       // for terminal nodes
    ParseTree(Symbol s, std::vector<ParseTree>& children);        // for non-terminal nodes
    ParseTree(Symbol s, std::vector<ParseTree>&& children = {});  // for non-terminal nodes
    const Token& data() const;
    const std::vector<ParseTree>& children() const;

    friend class Parser;
};
}  // namespace lang
#endif