#ifndef _GRAMMAR_H_
#define _GRAMMAR_H_

#include <vector>

#include "rule.h"
#include "symbol.h"

class Grammar {
    const Symbol _start;
    const std::vector<Rule> _rules;

   public:
    Grammar(Symbol start, std::vector<Rule> rules);
    Symbol start() const;
    const std::vector<Rule>& rules() const;
};

#endif