#include "parser.h"

#include <algorithm>
#include <utility>

#include "language/lexing/symbol.h"
#include "language/lexing/token.h"
namespace lang {

Parser::Parser(const Grammar* grammar) : grammar{grammar} {}

// ancillary function to add items if they are not already present
template <class T, class U>
void add_if_not_present(std::vector<std::pair<T, U>>& items, T t, U u) {
    for (const auto& item : items) {
        if (item.first == t) {
            return;
        }
    }

    items.emplace_back(t, u);
}

Parser::Tree::Tree(Token token) : _data{token}, _children{}, _parent{nullptr} {}

Parser::Tree::Tree(Symbol s, std::vector<std::unique_ptr<Tree>>&& children)
    : _data{Token{"", s}},
      _children{std::move(children)} {
    for (auto& child : _children) {
        child->_parent = this;
    }
}

const std::vector<std::unique_ptr<Parser::Tree>>& Parser::Tree::children() const {
    return _children;
}

const Token& Parser::Tree::data() const {
    return _data;
}

const Parser::Tree* Parser::Tree::parent() const {
    return _parent;
}

std::string Parser::Tree::to_string() const {
    std::string result = _data.value();
    if (!_children.empty()) {
        for (size_t i = 0; i < _children.size(); ++i) {
            result += _children[i]->to_string();
        }
    }
    return result;
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

std::unique_ptr<Parser::Tree> Parser::to_tree(const TempTree& temp_tree) {
    if (temp_tree.children.empty()) {
        return std::unique_ptr<Tree>(new Tree{temp_tree.data});
    }

    std::vector<std::unique_ptr<Tree>> children;

    for (const TempTree& child : temp_tree.children) {
        children.push_back(to_tree(child));
    }

    return std::unique_ptr<Tree>(new Tree{temp_tree.data.type(), std::move(children)});
}

std::unique_ptr<Parser::Tree> Parser::parse(const std::vector<Token>& tokens) {
    std::vector<std::vector<std::pair<Item, TempTree>>> state_sets;

    for (size_t i = 0; i <= tokens.size(); ++i) {
        state_sets.push_back({});
    }

    for (const Rule& rule : grammar->rules()) {
        if (rule.lhs() == grammar->start()) {
            state_sets[0].emplace_back(Item{&rule, 0, 0}, TempTree{rule.lhs()});
        }
    }

    // all tokens with index less than i have been processed
    for (size_t i = 0; i <= tokens.size(); ++i) {
        for (size_t j = 0; j < state_sets[i].size(); ++j) {
            std::pair<Item, TempTree> item = state_sets[i][j];

            // prediction
            if (!item.first.done() && !is_terminal(item.first.rule->rhs()[item.first.dot])) {
                Symbol next_symbol = item.first.rule->rhs()[item.first.dot];

                for (const Rule& rule : grammar->rules()) {
                    // add all rules with the next symbol on the left hand side
                    if (rule.lhs() == next_symbol) {
                        add_if_not_present(state_sets[i], {&rule, 0, i}, {rule.lhs()});
                    }
                }

                // if next symbol is nullable, advance the current rule
                if (grammar->nullable(next_symbol)) {
                    item.second.children.push_back(TempTree{next_symbol});

                    add_if_not_present(state_sets[i], {item.first.rule, item.first.dot + 1, item.first.start},
                                       {item.first.rule->lhs(), std::move(item.second.children)});
                }
            }

            // scanning
            if (!item.first.done() && is_terminal(item.first.rule->rhs()[item.first.dot])) {
                // check if the token matches the expected terminal
                // if it does, add the item to the next state set
                if ((i < tokens.size()) && (item.first.rule->rhs()[item.first.dot] == tokens[i].type())) {
                    item.second.children.push_back(TempTree{tokens[i]});
                    add_if_not_present(state_sets[i + 1],
                                       {item.first.rule, item.first.dot + 1, item.first.start},
                                       {item.first.rule->lhs(), std::move(item.second.children)});
                }
            }

            // completion
            if (item.first.done()) {
                // advance all items in the relevant state set that expect the completed item's LHS
                for (size_t k = 0; k < state_sets[item.first.start].size(); ++k) {
                    const std::pair<Item, TempTree>& start_item = state_sets[item.first.start][k];

                    if ((!start_item.first.done()) &&
                        (!start_item.first.rule->rhs().empty()) &&
                        (start_item.first.rule->rhs()[start_item.first.dot] == item.first.rule->lhs())) {
                        std::vector<TempTree> children = start_item.second.children;
                        children.push_back(item.second);
                        add_if_not_present(state_sets[i], {start_item.first.rule, start_item.first.dot + 1, start_item.first.start},
                                           {start_item.first.rule->lhs(), std::move(children)});
                    }
                }
            }
        }
    }

    // check if the last state set contains a completed item with the start symbol
    for (const std::pair<Item, TempTree>& item : state_sets.back()) {
        if (item.first.done() && (item.first.rule->lhs() == grammar->start()) && (item.first.start == 0)) {
            return to_tree(item.second);
        }
    }
    return std::unique_ptr<Tree>{};
}
}  // namespace lang
