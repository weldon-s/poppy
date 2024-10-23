#include "typer.h"

#include <cassert>
#include <format>

#include "language/lexing/symbol.h"

namespace lang {
bool Typer::SymbolTableKey::operator<(const SymbolTableKey& other) const {
    if (name != other.name) {
        return name < other.name;
    }

    return tree < other.tree;
}

Typer::Typer(Parser::Tree* tree) : _tree{tree} {
    construct(*_tree);
}

void Typer::add_symbol(std::string name, Type type, const Parser::Tree* tree) {
    auto ret = _symbol_table.insert({SymbolTableKey{name, tree}, type});

    if (!ret.second) {
        // no duplicate symbols allowed
        throw std::runtime_error(std::format("Duplicate symbol {} in symbol table", name));
    }
}

Type Typer::get_type(std::string name, const Parser::Tree& tree) const {
    const Parser::Tree* current_tree = &tree;

    while (current_tree) {
        SymbolTableKey key{name, current_tree};
        if (_symbol_table.contains(key)) {
            return _symbol_table.at(key);
        }

        current_tree = current_tree->parent();
    }

    throw std::runtime_error(std::format("Cannot find type of {} in the given tree", name));
}

Type symbol_to_type(const Parser::Tree& tree) {
    assert(tree.data().type() == Symbol::TYPE);

    switch (tree.children().front()->data().type()) {
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
    const Parser::Tree* params = tree.children()[3].get();  // OPTPARAMS
    const Parser::Tree& return_type_tree = *tree.children().front();

    Type return_type = symbol_to_type(return_type_tree);

    if (params->children().empty()) {
        return Type::function(return_type, {});
    }

    std::vector<Type> param_types;

    params = params->children().front().get();  // PARAMS

    // PARAMS -> PARAM | PARAM , PARAMS

    while (params->children().size() == 3) {
        const Parser::Tree* param = params->children().front().get();
        assert(param->data().type() == Symbol::PARAM);

        // PARAM -> TYPE IDENTIFIER
        Type param_type = symbol_to_type(*param->children().front());
        std::string param_name = param->children().back()->data().value();

        param_types.push_back(param_type);

        add_symbol(param_name, param_type, &tree);

        params = params->children()[2].get();
    }

    const Parser::Tree* param = params->children().front().get();
    assert(param->data().type() == Symbol::PARAM);

    // PARAM -> TYPE IDENTIFIER
    Type param_type = symbol_to_type(*param->children().front());
    std::string param_name = param->children().back()->data().value();

    param_types.push_back(param_type);

    add_symbol(param_name, param_type, &tree);

    return Type::function(return_type, param_types);
}

Type Typer::construct_program_tree(const Parser::Tree& tree) {
    assert(tree.data().type() == Symbol::PROGRAM);

    const Parser::Tree* defns = tree.children()[0].get();

    // the relevant rule here is DEFNS -> DEFN DEFNS | DEFN

    // first pass: add all function definitions to the symbol table
    while (defns->children().size() == 2) {
        const Parser::Tree* defn = defns->children().front().get();
        const Parser::Tree* name = defn->children()[1].get();

        Type function_type = parse_params(*defn);
        add_symbol(name->data().value(), function_type, &tree);

        defns = defns->children().back().get();
    }

    // second pass: type check all function definitions
    defns = tree.children()[0].get();

    while (defns->children().size() == 2) {
        const Parser::Tree* defn = defns->children().front().get();

        construct_defn_tree(*defn);
        defns = defns->children().back().get();
    }

    construct_defn_tree(*defns->children().front());
    return Type::NONE;
}

Type Typer::construct_defn_tree(const Parser::Tree& tree) {
    assert(tree.data().type() == Symbol::DEFN);
    Type stmts = construct_stmts_tree(*tree.children()[6], tree);

    if (stmts != symbol_to_type(*tree.children().front())) {
        throw std::runtime_error("Return type mismatch in function " + tree.children()[1]->data().value());
    }

    return Type::NONE;
}

Type Typer::construct_stmts_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::STMTS);

    // STMTS -> STMT | STMT STMTS

    if (tree.children().size() == 2) {
        construct_stmt_tree(*tree.children().front(), defn);
        Type ret = construct_stmts_tree(*tree.children().back(), defn);
        return ret;  // statements rule 1
    }

    return construct_stmt_tree(*tree.children().front(), defn);
}

Type Typer::construct_stmt_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::STMT);
    if (tree.children().size() == 2) {  // SEMISTMT
        return construct_semistmt_tree(*tree.children().front(), defn);
    }

    // STMT -> IF ( COND ) { STMTS } OPTELSE
    if (tree.children().front()->data().type() == Symbol::IF) {
        return construct_if_tree(tree, defn);
    }

    // STMT -> WHILE ( COND ) { STMTS }
    if (tree.children().front()->data().type() == Symbol::WHILE) {
        return construct_while_tree(tree, defn);
    }

    // STMT -> FOR ( SEMISTMT COND SEMISTMT ) { STMTS }
    return construct_for_tree(tree, defn);
}

Type Typer::construct_semistmt_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::SEMISTMT);

    switch (tree.children().front()->data().type()) {
        case Symbol::VARDEC:
            // VARDEC -> TYPE IDENTIFIER ASSIGN EXPR
            return construct_vardec_tree(*tree.children().front(), defn);
        case Symbol::VARASST:
            // VARASST -> IDENTIFIER ASSIGN EXPR
            return construct_varasst_tree(*tree.children().front(), defn);
        case Symbol::RET:
            // RET -> hop EXPR
            return construct_ret_tree(*tree.children().front(), defn);  // statements rule 2
        case Symbol::EXPR:
            // EXPR
            return construct_expr_tree(*tree.children().front(), defn);
        default:
            throw std::runtime_error("Invalid SEMISTMT");
    }
}

Type Typer::construct_if_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::STMT);

    // STMT -> IF ( COND ) { STMTS } OPTELSE

    const Parser::Tree* cond = tree.children()[2].get();
    const Parser::Tree* stmts = tree.children()[5].get();
    const Parser::Tree* optelse = tree.children().back().get();

    Type cond_type = construct_cond_tree(*cond, defn);

    if (cond_type != Type::BOOL) {
        throw std::runtime_error("Non-boolean condition in if statement");
    }

    construct_stmts_tree(*stmts, defn);

    if (!optelse->children().empty()) {
        construct_stmts_tree(*optelse->children()[2], defn);
    }

    return Type::VOID;
}

Type Typer::construct_while_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::STMT);

    // STMT -> WHILE ( COND ) { STMTS }

    const Parser::Tree* cond = tree.children()[2].get();
    const Parser::Tree* stmts = tree.children()[5].get();

    Type cond_type = construct_cond_tree(*cond, defn);

    if (cond_type != Type::BOOL) {
        throw std::runtime_error("Non-boolean condition in while statement");
    }

    construct_stmts_tree(*stmts, defn);

    return Type::VOID;
}

Type Typer::construct_for_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::STMT);

    // STMT -> FOR ( SEMISTMT; COND; SEMISTMT ) { STMTS }
    const Parser::Tree* init = tree.children()[2].get();
    const Parser::Tree* cond = tree.children()[4].get();
    const Parser::Tree* update = tree.children()[6].get();
    const Parser::Tree* stmts = tree.children()[9].get();

    construct_semistmt_tree(*init, defn);
    Type cond_type = construct_cond_tree(*cond, defn);

    if (cond_type != Type::BOOL) {
        throw std::runtime_error("Non-boolean condition in for statement");
    }

    construct_semistmt_tree(*update, defn);
    construct_stmts_tree(*stmts, defn);

    return Type::VOID;
}

Type Typer::construct_cond_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::COND);

    return construct_andcond_tree(*tree.children().front(), defn);
}

Type Typer::construct_andcond_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::ANDCOND);

    if (tree.children().size() == 1) {
        // ANDCOND -> ORCOND
        return construct_orcond_tree(*tree.children().front(), defn);
    }

    // ANDCOND -> ANDCOND && ORCOND

    Type type1 = construct_andcond_tree(*tree.children().front(), defn);
    Type type2 = construct_orcond_tree(*tree.children().back(), defn);

    if (type1 != Type::BOOL || type2 != Type::BOOL) {
        throw std::runtime_error("Non-boolean operand in logical AND");
    }

    return Type::BOOL;
}

Type Typer::construct_orcond_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::ORCOND);

    if (tree.children().size() == 1) {
        // ORCOND -> UNCOND
        return construct_uncond_tree(*tree.children().front(), defn);
    }

    // ORCOND -> ORCOND || UNCOND

    Type type1 = construct_orcond_tree(*tree.children().front(), defn);
    Type type2 = construct_uncond_tree(*tree.children().back(), defn);

    if (type1 != Type::BOOL || type2 != Type::BOOL) {
        throw std::runtime_error("Non-boolean operand in logical OR");
    }

    return Type::BOOL;
}

Type Typer::construct_uncond_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::UNCOND);

    if (tree.children().size() == 2) {
        // UNCOND -> NOT UNCOND
        return construct_uncond_tree(*tree.children().back(), defn);
    }

    if (tree.children().front()->data().type() == Symbol::LPAREN) {
        // UNCOND -> ( COND )
        return construct_cond_tree(*tree.children()[1], defn);
    }

    // UNCOND -> EXPR LT EXPR
    // UNCOND -> EXPR GT EXPR
    // UNCOND -> EXPR LE EXPR
    // UNCOND -> EXPR GE EXPR
    // UNCOND -> EXPR EQ EXPR
    // UNCOND -> EXPR NE EXPR

    Type left_type = construct_expr_tree(*tree.children().front(), defn);
    Type right_type = construct_expr_tree(*tree.children().back(), defn);

    if (left_type != Type::INT || right_type != Type::INT) {
        throw std::runtime_error("Invalid comparison type");
    }

    return Type::BOOL;
}

Type Typer::construct_ret_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::RET);
    Type ret_type = construct_expr_tree(*tree.children().back(), defn);

    if (ret_type == Type::VOID) {
        throw std::runtime_error("Return type mismatch");
    }

    return ret_type;
}

Type Typer::construct_vardec_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::VARDEC);

    // VARDEC -> TYPE IDENTIFIER ASSIGN EXPR
    const Parser::Tree* type = tree.children().front().get();
    const Parser::Tree* name = tree.children()[1].get();

    if (tree.children().size() == 2) {
        add_symbol(name->data().value(), symbol_to_type(*type), &defn);  // variables rule 1
        return Type::VOID;
    }

    const Parser::Tree* expr = tree.children().back().get();

    Type type_type = symbol_to_type(*type);
    Type expr_type = construct_expr_tree(*expr, defn);

    if (type_type != expr_type) {
        throw std::runtime_error("Type mismatch in declaration of " + name->data().value());
    }

    add_symbol(name->data().value(), type_type, &defn);  // variables rule 2
    return Type::VOID;
}

Type Typer::construct_varasst_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::VARASST);

    // VARASST -> IDENTIFIER ASSIGN EXPR

    const Parser::Tree* name = tree.children().front().get();
    const Parser::Tree* expr = tree.children().back().get();

    Type expr_type = construct_expr_tree(*expr, defn);
    Type name_type = get_type(name->data().value(), defn);

    if (name_type != expr_type) {
        throw std::runtime_error("Type mismatch in assignment to " + name->data().value());
    }

    return Type::VOID;
}

Type Typer::construct_expr_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::EXPR);

    // EXPR -> ADDEXPR
    return construct_addexpr_tree(*tree.children().front(), defn);
}

Type Typer::construct_addexpr_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::ADDEXPR);

    if (tree.children().size() == 1) {
        // ADD_EXPR -> MULTEXPR
        return construct_multexpr_tree(*tree.children().front(), defn);
    }

    if (tree.children()[1]->data().type() == Symbol::PLUS) {
        // ADD_EXPR -> ADD_EXPR + MULTEXPR
        Type left_type = construct_addexpr_tree(*tree.children().front(), defn);
        Type right_type = construct_multexpr_tree(*tree.children().back(), defn);

        if (left_type == Type::INT && right_type == Type::INT) {
            return Type::INT;  // arithmetic rule 1
        }

        if (left_type == Type::STRING && right_type == Type::STRING) {
            return Type::STRING;  // string manipulation rule 1
        }

        if (left_type == Type::STRING && right_type == Type::INT) {
            return Type::CHAR;  // string manipulation rule 2
        }

        throw std::runtime_error("Type mismatch in addition");
    }

    // ADD_EXPR -> ADD_EXPR - MULTEXPR

    Type left_type = construct_addexpr_tree(*tree.children().front(), defn);
    Type right_type = construct_multexpr_tree(*tree.children().back(), defn);

    if (left_type == Type::INT && right_type == Type::INT) {
        return Type::INT;  // arithmetic rule 1
    } else {
        throw std::runtime_error("Type mismatch in subtraction");
    }
}

Type Typer::construct_multexpr_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::MULTEXPR);

    if (tree.children().size() == 1) {
        // MULTEXPR -> UNEXPR
        return construct_unexpr_tree(*tree.children().front(), defn);
    }

    // MULTEXPR -> MULTEXPR * UNEXPR
    // MULTEXPR -> MULTEXPR / UNEXPR
    // MULTEXPR -> MULTEXPR % UNEXPR

    Type left_type = construct_multexpr_tree(*tree.children().front(), defn);
    Type right_type = construct_unexpr_tree(*tree.children().back(), defn);

    if (left_type == Type::INT && right_type == Type::INT) {
        return Type::INT;  // arithmetic rule 1
    } else {
        throw std::runtime_error("Type mismatch in multiplication, division, or modulo");
    }
}

Type Typer::construct_unexpr_tree(const Parser::Tree& tree, const Parser::Tree& defn) {
    assert(tree.data().type() == Symbol::UNEXPR);

    if (tree.children().front()->data().type() == Symbol::LPAREN) {
        // UNEXPR -> ( EXPR )
        return construct_expr_tree(*tree.children()[1], defn);  // arithmetic rule 2
    }

    if (tree.children().size() == 1) {
        switch (tree.children().front()->data().type()) {
            case Symbol::IDENTIFIER:
                // UNEXPR -> IDENTIFIER
                return get_type(tree.children().front()->data().value(), defn);  // variables rule 3
            case Symbol::CONSTANT:
                // UNEXPR -> CONSTANT
                return Type::INT;  // constants rule 1
            case Symbol::STRINGLIT:
                // UNEXPR -> STRINGLIT
                return Type::STRING;  // string literals rule 1
            case Symbol::CHARLIT:
                // UNEXPR -> CHARLIT
                return Type::CHAR;  // character literals rule 1
            default:
                throw std::runtime_error("Invalid UNEXPR");
        }
    } else if (tree.children().size() == 2) {
        // UNEXPR -> INC IDENTIFIER
        // UNEXPR -> DEC IDENTIFIER
        Type var_type = get_type(tree.children().back()->data().value(), defn);  // variables rule 4
        if (var_type != Type::INT) {
            throw std::runtime_error("Type mismatch in increment or decrement");
        }
        return Type::INT;
    }

    // UNEXPR -> IDENTIFIER ( OPTARGS )

    const Parser::Tree* name = tree.children().front().get();
    const Parser::Tree* args = tree.children()[2].get();

    Type function_type = get_type(name->data().value(), defn);

    if (!function_type.is_function()) {
        throw std::runtime_error("Trying to call non-function " + name->data().value());
    }

    std::vector<Type> arg_types;

    if (args->children().empty()) {
        if (function_type.arg_types().empty()) {
            return function_type.return_type();  // function calls rule 2
        }

        throw std::runtime_error("Type mismatch in function call");
    }

    args = args->children().front().get();  // ARGS

    while (args->children().size() == 3) {
        Type arg_type = construct_expr_tree(*args->children().front(), defn);
        arg_types.push_back(arg_type);
        args = args->children().back().get();
    }

    Type arg_type = construct_expr_tree(*args->children().front(), defn);
    arg_types.push_back(arg_type);

    if (arg_types != function_type.arg_types()) {
        throw std::runtime_error("Type mismatch in function call");
    }

    return function_type.return_type();  // function calls rule 1
}

Type Typer::construct(const Parser::Tree& tree) {
    return construct_program_tree(tree);
}
}  // namespace lang