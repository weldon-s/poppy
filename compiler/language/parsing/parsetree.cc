#include "parsetree.h"

#include <numeric>

namespace lang {
ParseTree::ParseTree(Token token, size_t index) : _data{token}, _children{}, start_index{index}, end_index{index + 1} {}

ParseTree::ParseTree(Symbol s, size_t index) : _data{Token{"", s}}, _children{}, start_index{index}, end_index{index} {}

ParseTree::ParseTree(Symbol s, std::vector<ParseTree> children)
    // concatenate the values of the children to form the value of the parent
    : _data{Token{std::accumulate(children.begin(), children.end(), std::string(""),
                                  [](std::string a, lang::ParseTree b) {
                                      return a + b._data.value();
                                  }),
                  s}},
      _children{children},
      start_index{children.front().start_index},
      end_index{children.back().end_index} {}

const std::vector<ParseTree>& ParseTree::children() const {
    return _children;
}

const Token& ParseTree::data() const {
    return _data;
}

size_t ParseTree::start() const {
    return start_index;
}

size_t ParseTree::end() const {
    return end_index;
}

}  // namespace lang
