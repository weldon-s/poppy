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

    std::map<const Parser::Tree*, lang::Type> _type_table;
    Parser::Tree _tree;

    void construct(const Parser::Tree&);
    void construct_program_tree(const Parser::Tree&);
    void construct_defn_tree(const Parser::Tree&);
    void construct_stmts_tree(const Parser::Tree&, const Parser::Tree&);
    void construct_stmt_tree(const Parser::Tree&, const Parser::Tree&);
    void construct_ret_tree(const Parser::Tree&, const Parser::Tree&);
    void construct_expr_tree(const Parser::Tree&, const Parser::Tree&);
    void construct_vardec_tree(const Parser::Tree&, const Parser::Tree&);
    void construct_varasst_tree(const Parser::Tree&, const Parser::Tree&);
    void construct_addexpr_tree(const Parser::Tree&, const Parser::Tree&);
    void construct_multexpr_tree(const Parser::Tree&, const Parser::Tree&);
    void construct_unexpr_tree(const Parser::Tree&, const Parser::Tree&);
    void construct_if_tree(const Parser::Tree&, const Parser::Tree&);
    void construct_while_tree(const Parser::Tree&, const Parser::Tree&);
    void construct_for_tree(const Parser::Tree&, const Parser::Tree&);
    void construct_cond_tree(const Parser::Tree&, const Parser::Tree&);
    void construct_andcond_tree(const Parser::Tree&, const Parser::Tree&);
    void construct_orcond_tree(const Parser::Tree&, const Parser::Tree&);
    void construct_uncond_tree(const Parser::Tree&, const Parser::Tree&);
    void construct_semistmt_tree(const Parser::Tree&, const Parser::Tree&);

    void add_symbol(std::string, Type, const Parser::Tree*);
    Type parse_params(const Parser::Tree&);
    Type get_type(std::string, const Parser::Tree&);

   public:
    Typer(Parser::Tree);
};
}  // namespace lang

#endif