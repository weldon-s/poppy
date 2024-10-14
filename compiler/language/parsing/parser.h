#ifndef _PARSER_H_
#define _PARSER_H_

#include <optional>

#include "grammar.h"
#include "language/lexing/token.h"
namespace lang {
class Parser {
    const Grammar* grammar;

   public:
    class Tree {
        const Token _data;
        std::vector<Tree> _children;

        Tree(Token token);                                  // for terminal nodes
        Tree(Symbol s, std::vector<Tree>& children);        // for non-terminal nodes
        Tree(Symbol s, std::vector<Tree>&& children = {});  // for non-terminal nodes

       public:
        const Token& data() const;
        const std::vector<Tree>& children() const;

        friend class Parser;
    };

    Parser(const Grammar* grammar);
    std::optional<Tree> parse(const std::vector<Token>& tokens);
};
}  // namespace lang
#endif