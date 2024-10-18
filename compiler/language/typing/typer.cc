#include "typer.h"

#include <cassert>
#include <format>
#include <iostream>

#include "language/lexing/symbol.h"

namespace lang {
bool Typer::SymbolTableKey::operator<(const SymbolTableKey& other) const {
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
    auto ret = _symbol_table.insert({SymbolTableKey{name, tree}, type});

    if (!ret.second) {
        // no duplicate symbols allowed
        throw std::runtime_error(std::format("Duplicate symbol {} in symbol table", name));
    }
}

Type Typer::get_type(std::string name, const Parser::Tree& tree) {
    const Parser::Tree* current_tree = &tree;

    while (current_tree) {
        SymbolTableKey key{name, current_tree};
        if (_symbol_table.contains(key)) {
            Type t = _symbol_table.at(key);
            return t;
        }
    }

    throw std::runtime_error("Use of undeclared variable " + name);
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

    construct_stmts_tree(*inner, tree);

    if (_type_table.at(inner) != symbol_to_type(tree.children().front())) {
        throw std::runtime_error("Return type mismatch in function " + tree.children()[1].data().value());
    }
}

void Typer::construct_stmts_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::STMTS);

    // STMTS -> STMT | STMT STMTS

    if (tree.children().size() == 2) {
        construct_stmt_tree(tree.children().front(), defn);
        construct_stmts_tree(tree.children().back(), defn);
        _type_table.emplace(&tree, _type_table.at(&tree.children().back()));  // statements rule 1
    } else {
        construct_stmt_tree(tree.children().front(), defn);
        _type_table.emplace(&tree, _type_table.at(&tree.children().front()));
    }
}

void Typer::construct_stmt_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::STMT);
    std::cout << tree.to_string() << std::endl;
    if (tree.children().size() == 2) {  // SEMISTMT
        construct_semistmt_tree(tree.children().front(), defn);
    }

    // STMT -> IF ( COND ) { STMTS } OPTELSE
    if (tree.children().front().data().type() == Symbol::IF) {
        construct_if_tree(tree, defn);
        return;
    }

    // STMT -> WHILE ( COND ) { STMTS }
    if (tree.children().front().data().type() == Symbol::WHILE) {
        construct_while_tree(tree, defn);
        return;
    }

    // STMT -> FOR ( SEMISTMT COND SEMISTMT ) { STMTS }
    construct_for_tree(tree, defn);
}

void Typer::construct_semistmt_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    switch (tree.children().front().data().type()) {
        case Symbol::VARDEC:
            // VARDEC -> TYPE IDENTIFIER ASSIGN EXPR
            construct_vardec_tree(tree.children().front(), defn);
            break;
        case Symbol::VARASST:
            // VARASST -> IDENTIFIER ASSIGN EXPR
            construct_varasst_tree(tree.children().front(), defn);
            break;
        case Symbol::RET:
            construct_ret_tree(tree.children().front(), defn);
            _type_table.emplace(&tree, _type_table.at(&tree.children().front()));  // statements rule 2
            break;
        case Symbol::EXPR:
            // EXPR
            construct_expr_tree(tree.children().front(), defn);
            break;
        default:
            throw std::runtime_error("Invalid semistmt");
    }

    if (!_type_table.contains(&tree)) {
        _type_table.emplace(&tree, Type::VOID);
    }
}

void Typer::construct_if_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::STMT);

    // STMT -> IF ( COND ) { STMTS } OPTELSE

    const Parser::Tree* cond = &tree.children()[2];
    const Parser::Tree* stmts = &tree.children()[5];
    const Parser::Tree* optelse = &tree.children().back();

    construct_cond_tree(*cond, defn);

    if (_type_table.at(cond) != Type::BOOL) {
        throw std::runtime_error("Non-boolean condition in if statement");
    }

    construct_stmts_tree(*stmts, defn);

    if (!optelse->children().empty()) {
        construct_stmts_tree(optelse->children()[2], defn);
    }

    _type_table.emplace(&tree, Type::VOID);
}

void Typer::construct_while_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::STMT);

    // STMT -> WHILE ( COND ) { STMTS }

    const Parser::Tree* cond = &tree.children()[2];
    const Parser::Tree* stmts = &tree.children()[5];

    construct_cond_tree(*cond, defn);

    if (_type_table.at(cond) != Type::BOOL) {
        throw std::runtime_error("Non-boolean condition in while statement");
    }

    construct_stmts_tree(*stmts, defn);

    _type_table.emplace(&tree, Type::VOID);
}

void Typer::construct_for_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::STMT);

    // STMT -> FOR ( SEMISTMT; COND; SEMISTMT ) { STMTS }
    const Parser::Tree* init = &tree.children()[2];
    const Parser::Tree* cond = &tree.children()[4];
    const Parser::Tree* update = &tree.children()[6];
    const Parser::Tree* stmts = &tree.children()[9];

    construct_semistmt_tree(*init, defn);
    construct_cond_tree(*cond, defn);

    if (_type_table.at(cond) != Type::BOOL) {
        throw std::runtime_error("Non-boolean condition in for statement");
    }

    construct_semistmt_tree(*update, defn);
    construct_stmts_tree(*stmts, defn);

    _type_table.emplace(&tree, Type::VOID);
}

void Typer::construct_cond_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::COND);

    construct_andcond_tree(tree.children().front(), defn);
    _type_table.emplace(&tree, _type_table.at(&tree.children().front()));
}

void Typer::construct_andcond_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::ANDCOND);

    if (tree.children().size() == 1) {
        // ANDCOND -> ORCOND
        construct_orcond_tree(tree.children().front(), defn);
        _type_table.emplace(&tree, _type_table.at(&tree.children().front()));
        return;
    }

    // ANDCOND -> ANDCOND && ORCOND

    construct_andcond_tree(tree.children().front(), defn);
    construct_orcond_tree(tree.children().back(), defn);

    if (_type_table.at(&tree.children().front()) != Type::BOOL || _type_table.at(&tree.children().back()) != Type::BOOL) {
        throw std::runtime_error("Non-boolean operand in logical AND");
    }

    _type_table.emplace(&tree, Type::BOOL);
}

void Typer::construct_orcond_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::ORCOND);

    if (tree.children().size() == 1) {
        // ORCOND -> UNCOND
        construct_uncond_tree(tree.children().front(), defn);
        _type_table.emplace(&tree, _type_table.at(&tree.children().front()));
        return;
    }

    // ORCOND -> ORCOND || UNCOND

    construct_orcond_tree(tree.children().front(), defn);
    construct_uncond_tree(tree.children().back(), defn);

    if (_type_table.at(&tree.children().front()) != Type::BOOL || _type_table.at(&tree.children().back()) != Type::BOOL) {
        throw std::runtime_error("Non-boolean operand in logical OR");
    }

    _type_table.emplace(&tree, Type::BOOL);
}

void Typer::construct_uncond_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::UNCOND);

    if (tree.children().size() == 2) {
        // UNCOND -> NOT UNCOND
        construct_uncond_tree(tree.children().back(), defn);

        if (_type_table.at(&tree.children().back()) != Type::BOOL) {
            throw std::runtime_error("Non-boolean operand in logical NOT");
        }

        _type_table.emplace(&tree, Type::BOOL);
        return;
    }

    if (tree.children().front().data().type() == Symbol::LPAREN) {
        // UNCOND -> ( COND )
        construct_cond_tree(tree.children()[1], defn);
        _type_table.emplace(&tree, _type_table.at(&tree.children()[1]));
        return;
    }

    // UNCOND -> EXPR LT EXPR
    // UNCOND -> EXPR GT EXPR
    // UNCOND -> EXPR LE EXPR
    // UNCOND -> EXPR GE EXPR
    // UNCOND -> EXPR EQ EXPR
    // UNCOND -> EXPR NE EXPR

    construct_expr_tree(tree.children().front(), defn);
    construct_expr_tree(tree.children().back(), defn);

    Type left_type = _type_table.at(&tree.children().front());
    Type right_type = _type_table.at(&tree.children().back());

    if (left_type != Type::INT || right_type != Type::INT) {
        throw std::runtime_error("Invalid comparison type");
    }

    _type_table.emplace(&tree, Type::BOOL);
}

void Typer::construct_ret_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    construct_expr_tree(tree.children().back(), defn);
    _type_table.emplace(&tree, _type_table.at(&tree.children().back()));
}

void Typer::construct_vardec_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::VARDEC);

    // VARDEC -> TYPE IDENTIFIER ASSIGN EXPR
    const Parser::Tree* type = &tree.children().front();
    const Parser::Tree* name = &tree.children()[1];

    if (tree.children().size() == 2) {
        add_symbol(name->data().value(), symbol_to_type(*type), &defn);  // variables rule 1
        return;
    }

    const Parser::Tree* expr = &tree.children().back();

    Type type_type = symbol_to_type(*type);
    construct_expr_tree(*expr, defn);

    if (type_type != _type_table.at(expr)) {
        throw std::runtime_error("Type mismatch in declaration of " + name->data().value());
    }

    add_symbol(name->data().value(), type_type, &tree);  // variables rule 2
}

void Typer::construct_varasst_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::VARASST);

    // VARASST -> IDENTIFIER ASSIGN EXPR

    const Parser::Tree* name = &tree.children().front();
    const Parser::Tree* expr = &tree.children().back();

    construct_expr_tree(*expr, defn);
    Type name_type = get_type(name->data().value(), defn);

    if (name_type != _type_table.at(expr)) {
        throw std::runtime_error("Type mismatch in assignment to " + name->data().value());
    }
}

void Typer::construct_expr_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    std::cout << "Expr: " << tree.to_string() << std::endl;

    std::cout << (int)tree.data().type() << std::endl;

    assert(tree.data().type() == Symbol::EXPR);

    // EXPR -> ADDEXPR
    construct_addexpr_tree(tree.children().front(), defn);
    _type_table.emplace(&tree, _type_table.at(&tree.children().front()));
}

void Typer::construct_addexpr_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::ADDEXPR);

    if (tree.children().size() == 1) {
        // ADD_EXPR -> MULTEXPR
        construct_multexpr_tree(tree.children().front(), defn);
        _type_table.emplace(&tree, _type_table.at(&tree.children().front()));
        return;
    }

    if (tree.children()[1].data().type() == Symbol::PLUS) {
        // ADD_EXPR -> ADD_EXPR + MULTEXPR
        construct_addexpr_tree(tree.children().front(), defn);
        construct_multexpr_tree(tree.children().back(), defn);

        Type left_type = _type_table.at(&tree.children().front());
        Type right_type = _type_table.at(&tree.children().back());

        if (left_type == Type::INT && right_type == Type::INT) {
            _type_table.emplace(&tree, Type::INT);  // arithmetic rule 1
        } else if (left_type == Type::STRING && right_type == Type::STRING) {
            _type_table.emplace(&tree, Type::STRING);  // string manipulation rule 1
        } else if (left_type == Type::STRING && right_type == Type::INT) {
            _type_table.emplace(&tree, Type::CHAR);  // string manipulation rule 2
        } else {
            throw std::runtime_error("Type mismatch in addition");
        }
        return;
    }

    // ADD_EXPR -> ADD_EXPR - MULTEXPR

    construct_addexpr_tree(tree.children().front(), defn);
    construct_multexpr_tree(tree.children().back(), defn);

    Type left_type = _type_table.at(&tree.children().front());
    Type right_type = _type_table.at(&tree.children().back());

    if (left_type == Type::INT && right_type == Type::INT) {
        _type_table.emplace(&tree, Type::INT);  // arithmetic rule 1
    } else {
        throw std::runtime_error("Type mismatch in subtraction");
    }
}

void Typer::construct_multexpr_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::MULTEXPR);

    if (tree.children().size() == 1) {
        // MULTEXPR -> UNEXPR
        construct_unexpr_tree(tree.children().front(), defn);
        _type_table.emplace(&tree, _type_table.at(&tree.children().front()));
        return;
    }

    // MULTEXPR -> MULTEXPR * UNEXPR
    // MULTEXPR -> MULTEXPR / UNEXPR
    // MULTEXPR -> MULTEXPR % UNEXPR

    construct_multexpr_tree(tree.children().front(), defn);
    construct_unexpr_tree(tree.children().back(), defn);

    Type left_type = _type_table.at(&tree.children().front());
    Type right_type = _type_table.at(&tree.children().back());

    if (left_type == Type::INT && right_type == Type::INT) {
        _type_table.emplace(&tree, Type::INT);  // arithmetic rule 1
    } else {
        throw std::runtime_error("Type mismatch in multiplication, division, or modulo");
    }
}

void Typer::construct_unexpr_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::UNEXPR);

    if (tree.children().front().data().type() == Symbol::LPAREN) {
        // UNEXPR -> ( EXPR )
        construct_expr_tree(tree.children()[1], defn);
        _type_table.emplace(&tree, _type_table.at(&tree.children()[1]));  // arithmetic rule 2
        return;
    } else if (tree.children().size() == 1) {
        switch (tree.children().front().data().type()) {
            case Symbol::IDENTIFIER:
                // UNEXPR -> IDENTIFIER
                _type_table.emplace(&tree, get_type(tree.children().front().data().value(), defn));  // variables rule 3
                return;
            case Symbol::CONSTANT:
                // UNEXPR -> CONSTANT
                _type_table.emplace(&tree, Type::INT);  // constants rule 1
                return;
            case Symbol::STRINGLIT:
                // UNEXPR -> STRINGLIT
                _type_table.emplace(&tree, Type::STRING);  // string literals rule 1
                return;
            case Symbol::CHARLIT:
                // UNEXPR -> CHARLIT
                _type_table.emplace(&tree, Type::CHAR);  // character literals rule 1
                return;
            default:
                throw std::runtime_error("Invalid UNEXPR");
        }
    } else if (tree.children().size() == 2) {
        // UNEXPR -> INC IDENTIFIER
        // UNEXPR -> DEC IDENTIFIER
        _type_table.emplace(&tree, get_type(tree.children().back().data().value(), defn));  // variables rule 4
        if (_type_table.at(&tree) != Type::INT) {
            throw std::runtime_error("Type mismatch in increment or decrement");
        }
        return;
    }

    // UNEXPR -> IDENTIFIER ( OPTARGS )

    const Parser::Tree* name = &tree.children().front();
    const Parser::Tree* args = &tree.children()[2];

    Type function_type = get_type(name->data().value(), defn);

    assert(function_type.is_function());

    std::vector<Type> arg_types;

    if (args->children().empty()) {
        if (function_type.arg_types().empty()) {
            _type_table.emplace(&tree, function_type.return_type());  // function calls rule 2
            return;
        }

        throw std::runtime_error("Type mismatch in function call");
    }

    args = &args->children().front();  // ARGS

    while (args->children().size() == 3) {
        construct_expr_tree(args->children().front(), defn);
        arg_types.push_back(_type_table.at(&args->children().front()));
        args = &args->children().back();
    }

    construct_expr_tree(args->children().front(), defn);
    arg_types.push_back(_type_table.at(&args->children().front()));

    if (arg_types != function_type.arg_types()) {
        throw std::runtime_error("Type mismatch in function call");
    }

    _type_table.emplace(&tree, function_type.return_type());  // function calls rule 1
}

void Typer::construct(const Parser::Tree& tree) {
    construct_program_tree(tree);
}
}  // namespace lang