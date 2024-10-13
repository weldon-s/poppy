#ifndef _PARSETREE_H_
#define _PARSETREE_H_

#include <vector>

#include "language/lexing/token.h"

namespace lang {
enum class Symbol;

class ParseTree {
    const Token _data;
    const std::vector<ParseTree> _children;
    const size_t start_index;  // inclusive
    const size_t end_index;    // exclusive

   public:
    ParseTree(Token token, size_t index);                  // for terminal nodes
    ParseTree(Symbol s, size_t index);                     // for non-terminal nodes with no children
    ParseTree(Symbol s, std::vector<ParseTree> children);  // for non-terminal nodes
    const Token& data() const;
    const std::vector<ParseTree>& children() const;
    size_t start() const;
    size_t end() const;
};
}  // namespace lang
#endif