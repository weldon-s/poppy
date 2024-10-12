#ifndef _RULE_H_
#define _RULE_H_

#include <vector>

enum class Symbol;

class Rule {
    const Symbol _lhs;
    const std::vector<Symbol> _rhs;

   public:
    Rule(Symbol lhs, std::vector<Symbol> rhs);
    Symbol lhs() const;
    const std::vector<Symbol>& rhs() const;
};

#endif