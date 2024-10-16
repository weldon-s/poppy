#include "typer.h"

#include <cassert>
#include <format>
#include <iostream>

#include "language/lexing/symbol.h"

namespace lang {
bool Typer::SymbolTableEntry::operator<(const SymbolTableEntry& other) const {
    if (name != other.name) {
        return name < other.name;
    }

    return tree < other.tree;
}

Typer::Typer(Parser::Tree tree) : _tree{tree} {
    construct(_tree);
}

void Typer::add_symbol(std::string name, Type type, const Parser::Tree* tree) {
    std::cout << "Adding symbol " << name << " of type " << type.name() << std::endl;
    auto ret = _symbol_table.emplace(name, type, tree);

    if (!ret.second) {
        // no duplicate symbols allowed
        throw std::runtime_error(std::format("Duplicate symbol {} in symbol table", name));
    }
}

Type symbol_to_type(const Parser::Tree& tree) {
    assert(tree.data().type() == Symbol::TYPE);

    switch (tree.children().front().data().type()) {
        case Symbol::INT:
            return Type::INT;

        case Symbol::CHAR:
            return Type::CHAR;

        case Symbol::STRING:
            return Type::STRING;
        default:
            throw std::runtime_error("Invalid type");
    }
}

Type Typer::parse_params(const Parser::Tree& tree) {
    assert(tree.data().type() == Symbol::DEFN);
    const Parser::Tree* params = &tree.children()[3];  // OPTPARAMS
    const Parser::Tree& return_type_tree = tree.children().front();

    Type return_type = symbol_to_type(return_type_tree);

    if (params->children().empty()) {
        return Type::function(return_type, {});
    }

    std::vector<Type> param_types;

    params = &params->children().front();  // PARAMS

    // PARAMS -> PARAM | PARAM , PARAMS

    while (params->children().size() == 3) {
        const Parser::Tree* param = &params->children().front();
        assert(param->data().type() == Symbol::PARAM);

        // PARAM -> TYPE IDENTIFIER
        Type param_type = symbol_to_type(param->children().front());
        std::string param_name = param->children().back().data().value();

        param_types.push_back(param_type);

        add_symbol(param_name, param_type, &tree);

        params = &params->children()[2];
    }

    const Parser::Tree* param = &params->children().front();
    assert(param->data().type() == Symbol::PARAM);

    // PARAM -> TYPE IDENTIFIER
    Type param_type = symbol_to_type(param->children().front());
    std::string param_name = param->children().back().data().value();

    param_types.push_back(param_type);

    add_symbol(param_name, param_type, &tree);

    return Type::function(return_type, param_types);
}

void Typer::construct_program_tree(const Parser::Tree& tree) {
    assert(tree.data().type() == Symbol::PROGRAM);

    const Parser::Tree* defns = &tree.children()[0];

    // the relevant rule here is DEFNS -> DEFN DEFNS | DEFN

    // first pass: add all function definitions to the symbol table
    while (defns->children().size() == 2) {
        const Parser::Tree* defn = &defns->children().front();
        const Parser::Tree* name = &defn->children()[1];

        Type function_type = parse_params(*defn);
        add_symbol(name->data().value(), function_type, &tree);

        defns = &defns->children().back();
    }

    // second pass: type check all function definitions
    defns = &tree.children()[0];

    while (defns->children().size() == 2) {
        const Parser::Tree* defn = &defns->children().front();

        construct_defn_tree(*defn);
        defns = &defns->children().back();
    }

    construct_defn_tree(defns->children().front());
}

void Typer::construct_defn_tree(const Parser::Tree& tree) {
    assert(tree.data().type() == Symbol::DEFN);
    std::cout << "Defn: " << tree.to_string() << std::endl;
    const Parser::Tree* inner = &tree.children()[6];  // STMTS

    // STMTS -> STMT | STMT STMTS
    while ((inner->children().size() == 2)) {
        construct(inner->children().front());
        inner = &inner->children().back();
    }

    construct(inner->children().front());
}

void Typer::construct_stmt_tree(const Parser::Tree& tree) {
    assert(tree.data().type() == Symbol::STMT);
    std::cout << tree.to_string() << std::endl;
}

void Typer::construct(const Parser::Tree& tree) {
    switch (tree.data().type()) {
        case Symbol::PROGRAM:
            construct_program_tree(tree);
            break;
        case Symbol::DEFN:
            construct_defn_tree(tree);
            break;
        case Symbol::STMT:
            construct_stmt_tree(tree);
            break;
    }
}
}  // namespace lang