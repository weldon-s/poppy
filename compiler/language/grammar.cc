#include "grammar.h"

#include <algorithm>

#include "symbol.h"

Grammar::Grammar(Symbol start, std::vector<Rule> rules) : _start{start}, _rules{rules}, _nullable{} {
    for (size_t i = 0; i < _rules.size(); ++i) {
        bool current_nullable = true;

        for (const Symbol& s : _rules[i].rhs()) {
            if (std::find(_nullable.begin(), _nullable.end(), s) == _nullable.end()) {
                current_nullable = false;
                break;
            }
        }

        if (current_nullable && (std::find(_nullable.begin(), _nullable.end(), _rules[i].lhs()) == _nullable.end())) {
            _nullable.push_back(_rules[i].lhs());
        }
    }
}

Symbol Grammar::start() const {
    return _start;
}

const std::vector<Rule>& Grammar::rules() const {
    return _rules;
}

bool Grammar::nullable(Symbol s) const {
    return std::find(_nullable.begin(), _nullable.end(), s) != _nullable.end();
}