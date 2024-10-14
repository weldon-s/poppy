#include "parsetree.h"

#include <numeric>

namespace lang {
ParseTree::ParseTree(Token token) : _data{token}, _children{} {}

ParseTree::ParseTree(Symbol s, std::vector<ParseTree>& children)
    : _data{Token{"", s}},
      _children{children} {}

ParseTree::ParseTree(Symbol s, std::vector<ParseTree>&& children)
    : _data{Token{"", s}},
      _children{children} {}

const std::vector<ParseTree>& ParseTree::children() const {
    return _children;
}

const Token& ParseTree::data() const {
    return _data;
}

}  // namespace lang
