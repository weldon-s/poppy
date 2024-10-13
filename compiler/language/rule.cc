#include "rule.h"

namespace lang {
Rule::Rule(Symbol lhs, std::vector<Symbol> rhs) : _lhs{lhs}, _rhs{rhs} {}

Symbol Rule::lhs() const {
    return _lhs;
}

const std::vector<Symbol>& Rule::rhs() const {
    return _rhs;
}
}  // namespace lang