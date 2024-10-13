#include <iostream>

#include "language/lexer.h"
#include "language/parser.h"
#include "language/poppygrammar.h"

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

    std::cout << p.parse(tokens) << std::endl;
}