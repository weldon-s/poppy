#include <fstream>
#include <iostream>

#include "language/lexing/lexer.h"
#include "language/parsing/parser.h"
#include "language/parsing/poppygrammar.h"
#include "language/typing/typer.h"

int main(int argc, char* argv[]) {
    std::ifstream file{argv[1]};
    std::string contents{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    lang::Lexer l{contents};

    std::vector<lang::Token> tokens = l.scan();

    for (const auto& token : tokens) {
        std::cout << token.value() << std::endl;
    }

    lang::Parser p{&poppy_grammar};

    std::unique_ptr<Parser::Tree> tree = p.parse(tokens);

    if (!tree) {
        std::cout << "Parse error" << std::endl;
        return 1;
    }

    try {
        lang::Typer t{tree.get()};
    } catch (const std::exception& e) {
        std::cerr << "Error during typing: ";
        std::cerr << e.what() << std::endl;
    }
}