#include <iostream>

#include "language/lexing/lexer.h"
#include "language/parsing/parser.h"
#include "language/parsing/poppygrammar.h"

void print_tree(const Parser::Tree& tree, int depth) {
    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }
    std::cout << (int)tree.data().type() << ": ";
    std::cout << tree.data().value() << std::endl;

    for (const Parser::Tree& child : tree.children()) {
        print_tree(child, depth + 1);
    }
}

int main(int argc, char* argv[]) {
    lang::Lexer l{
        R"""(
    !!hello
    char f(int n, int m) {
        if (n <= 1) {
            hop n;
        }

        1 * 2 + d / 4 % 5050 - 'r';

        if (n == 2 || n == 3) {
            hop 1;
        }
        else {
            string s = "dddd";
            s = 4;
        }

        while(!(n > 0 && n <= 0 && n != 0)) {
            hop n;

            for(int i = 0; !i >= 10; i = i + 1) {
                print(i);
            }

            n = n - 1;
        }

        hop f(n - 'f') + f(f(--n) - 2);
    }

    !-long comment...
    words words words
    ...
    -!
    int main(){
        print(f(10));
        hop 0;
    }
            )"""};

    std::vector<lang::Token> tokens = l.scan();

    for (const auto& token : tokens) {
        std::cout << token.value() << std::endl;
    }

    lang::Parser p{&poppy_grammar};

    std::optional<Parser::Tree> tree = p.parse(tokens);

    if (tree) {
        print_tree(*tree, 0);
    } else {
        std::cout << "Parse error" << std::endl;
    }
}