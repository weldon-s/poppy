#include <iostream>

#include "language/lexer.h"

int main(int argc, char* argv[]) {
    Lexer l{
        R"""(
    !! hello
    !!
    int fib(int n){
        if (n <= 1){
            hop n;
        }
        !-
        long comment...
        -!

        hop fib(n - 1) + fib(n - 2);
    }

    int main(){
        print(fib(10));

        hop 0;
    }
            )"""};

    for (const auto& token : l.scan()) {
        std::cout << token.value() << std::endl;
    }
}