#ifndef _GRAMMAR_H_
#define _GRAMMAR_H_

#include <vector>

#include "rule.h"
namespace lang {

enum class Symbol;
class Grammar {
    const Symbol _start;
    const std::vector<Rule> _rules;
    std::vector<Rule> _nonempty_rules;
    std::vector<Symbol> _nullable;

   public:
    Grammar(Symbol start, std::vector<Rule> rules);
    Symbol start() const;
    const std::vector<Rule>& rules() const;
    const std::vector<Rule>& nonempty_rules() const;
    bool nullable(Symbol s) const;
};
}  // namespace lang

#endif