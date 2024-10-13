#include "parser.h"

#include <algorithm>

#include "token.h"

Parser::Parser(const Grammar* grammar) : grammar{grammar} {}

// ancillary function to add items if they are not already present
template <class T>
void add_if_not_present(std::vector<T>& items, T item) {
    if (std::find(items.begin(), items.end(), item) == items.end()) {
        items.push_back(item);
    }
}

// Earley item
struct Item {
    const Rule* rule;
    const size_t dot;  // position of the dot (occurs before the symbol at this index of rule's RHS)
    const size_t start;
    bool operator==(const Item& other) const {
        return (rule == other.rule) && (dot == other.dot) && (start == other.start);
    }

    bool done() const {
        return dot == rule->rhs().size();
    }
};

bool Parser::parse(const std::vector<Token>& tokens) {
    std::vector<std::vector<Item>> state_sets;

    for (size_t i = 0; i <= tokens.size(); ++i) {
        state_sets.push_back({});
    }

    for (const Rule& rule : grammar->rules()) {
        if (rule.lhs() == grammar->start()) {
            state_sets[0].emplace_back(&rule, 0, 0);
        }
    }

    // all tokens with index less than i have been processed
    for (size_t i = 0; i <= tokens.size(); ++i) {
        for (size_t j = 0; j < state_sets[i].size(); ++j) {
            const Item item = state_sets[i][j];

            // prediction
            if (!item.done() && !is_terminal(item.rule->rhs()[item.dot])) {
                Symbol next_symbol = item.rule->rhs()[item.dot];

                for (const Rule& rule : grammar->rules()) {
                    // add all rules with the next symbol on the left hand side
                    if (rule.lhs() == next_symbol) {
                        add_if_not_present(state_sets[i], {&rule, 0, i});
                    }
                }

                // if next symbol is nullable, advance the current rule
                if (grammar->nullable(next_symbol)) {
                    add_if_not_present(state_sets[i], {item.rule, item.dot + 1, item.start});
                }
            }

            // scanning
            if (!item.done() && is_terminal(item.rule->rhs()[item.dot])) {
                // check if the token matches the expected terminal
                // if it does, add the item to the next state set
                if ((i < tokens.size()) && (item.rule->rhs()[item.dot] == tokens[i].type())) {
                    add_if_not_present(state_sets[i + 1], {item.rule, item.dot + 1, item.start});
                }
            }

            // completion
            if (item.done()) {
                // advance all items in the relevant state set that expect the completed item's LHS
                for (size_t k = 0; k < state_sets[item.start].size(); ++k) {
                    const Item& start_item = state_sets[item.start][k];

                    if ((!start_item.done()) &&
                        (!start_item.rule->rhs().empty()) &&
                        (start_item.rule->rhs()[start_item.dot] == item.rule->lhs())) {
                        add_if_not_present(state_sets[i], {start_item.rule, start_item.dot + 1, start_item.start});
                    }
                }
            }
        }
    }

    // check if the last state set contains a completed item with the start symbol
    for (const Item& item : state_sets.back()) {
        if (item.done() && (item.rule->lhs() == grammar->start()) && (item.start == 0)) {
            return true;
        }
    }

    return false;
}
