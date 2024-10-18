#ifndef _PARSER_H_
#define _PARSER_H_

#include <memory>

#include "grammar.h"
#include "language/lexing/token.h"
namespace lang {
class Parser {
    const Grammar* grammar;

    struct TempTree {
        Token data;
        std::vector<TempTree> children;

        TempTree(Token token) : data{token}, children{} {}

        TempTree(Symbol s, std::vector<TempTree>& children)
            : data{Token{"", s}},
              children{children} {}

        TempTree(Symbol s, std::vector<TempTree>&& children = {})
            : data{Token{"", s}},
              children{std::move(children)} {}
    };

   public:
    class Tree {  // TODO see if this can be merged with TempTree
        const Token _data;
        std::vector<std::unique_ptr<Tree>> _children;
        Tree* _parent;

        Tree(Token token);                                                   // for terminal nodes
        Tree(Symbol s, std::vector<std::unique_ptr<Tree>>&& children = {});  // for non-terminal nodes

       public:
        const Token& data() const;
        const std::vector<std::unique_ptr<Tree>>& children() const;
        const Tree* parent() const;
        std::string to_string() const;

        friend class Parser;
    };

    Parser(const Grammar* grammar);
    std::unique_ptr<Tree> parse(const std::vector<Token>& tokens);

   private:
    std::unique_ptr<Tree> to_tree(const TempTree& temp_tree);
};
}  // namespace lang
#endif