#ifndef _TYPER_H_
#define _TYPER_H_

#include "language/parsing/parser.h"
#include "language/typing/type.h"

namespace lang {
class Typer {
    class Tree;

    Tree construct(const Parser::Tree& tree);

   public:
    class Tree {
        Type _type;
        std::vector<Tree> _children;
        std::optional<std::string> _value;  // for terminals

        Tree(Type type, std::vector<Tree>& children);
        Tree(Type type, std::string value);

        friend class Typer;

       public:
        Type type() const;
        const std::vector<Tree>& children() const;
        std::optional<std::string> value() const;
    };

    Typer(Parser::Tree tree);
};
}  // namespace lang

#endif