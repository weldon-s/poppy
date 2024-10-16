#ifndef _TYPER_H_
#define _TYPER_H_

#include <map>
#include <memory>
#include <set>

#include "language/parsing/parser.h"
#include "language/typing/type.h"

namespace lang {
class Typer {
    struct SymbolTableEntry {
        const std::string name;
        const lang::Type type;
        const Parser::Tree* tree;

        bool operator<(const SymbolTableEntry&) const;
    };

    std::set<SymbolTableEntry> _symbol_table;
    std::map<const Parser::Tree*, lang::Type> _type_table;
    Parser::Tree _tree;

    void construct(const Parser::Tree&);
    void construct_program_tree(const Parser::Tree&);
    void construct_defn_tree(const Parser::Tree&);
    void construct_stmt_tree(const Parser::Tree&);

    void add_symbol(std::string, Type, const Parser::Tree*);
    Type parse_params(const Parser::Tree&);

   public:
    Typer(Parser::Tree);
};
}  // namespace lang

#endif