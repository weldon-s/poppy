#ifndef _TYPER_H_
#define _TYPER_H_

#include <map>
#include <memory>

#include "language/parsing/parser.h"
#include "language/typing/type.h"

namespace lang {
class Typer {
    struct SymbolTableKey {
        const std::string name;
        const Parser::Tree* tree;

        bool operator<(const SymbolTableKey&) const;
    };

    std::map<SymbolTableKey, Type> _symbol_table;

    const Parser::Tree* _tree;

    Type construct(const Parser::Tree&);
    Type construct_program_tree(const Parser::Tree&);
    Type construct_defn_tree(const Parser::Tree&);
    Type construct_stmts_tree(const Parser::Tree&, const Parser::Tree&);
    Type construct_stmt_tree(const Parser::Tree&, const Parser::Tree&);
    Type construct_ret_tree(const Parser::Tree&, const Parser::Tree&);
    Type construct_expr_tree(const Parser::Tree&, const Parser::Tree&);
    Type construct_vardec_tree(const Parser::Tree&, const Parser::Tree&);
    Type construct_varasst_tree(const Parser::Tree&, const Parser::Tree&);
    Type construct_addexpr_tree(const Parser::Tree&, const Parser::Tree&);
    Type construct_multexpr_tree(const Parser::Tree&, const Parser::Tree&);
    Type construct_unexpr_tree(const Parser::Tree&, const Parser::Tree&);
    Type construct_if_tree(const Parser::Tree&, const Parser::Tree&);
    Type construct_while_tree(const Parser::Tree&, const Parser::Tree&);
    Type construct_for_tree(const Parser::Tree&, const Parser::Tree&);
    Type construct_cond_tree(const Parser::Tree&, const Parser::Tree&);
    Type construct_andcond_tree(const Parser::Tree&, const Parser::Tree&);
    Type construct_orcond_tree(const Parser::Tree&, const Parser::Tree&);
    Type construct_uncond_tree(const Parser::Tree&, const Parser::Tree&);
    Type construct_semistmt_tree(const Parser::Tree&, const Parser::Tree&);

    void add_symbol(std::string, Type, const Parser::Tree*);
    Type parse_params(const Parser::Tree&);
    Type get_type(std::string, const Parser::Tree&);

   public:
    Typer(Parser::Tree*);
};
}  // namespace lang

#endif