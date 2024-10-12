#include "grammar.h"

#include "symbol.h"

Grammar::Grammar(Symbol start, std::vector<Rule> rules) : _start{start}, _rules{rules} {}

Symbol Grammar::start() const {
    return _start;
}

const std::vector<Rule>& Grammar::rules() const {
    return _rules;
}
