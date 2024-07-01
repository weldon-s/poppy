#include "instruction.h"
#include "modules/popipo.h"
#include "program.h"

int main() {
    Program program;

    program.add_include("print_num")
        .add_code(print_str("hi", "Hi, world!\n"))
        .add_code(print_num(4))
        .add_code(print_str("hello", "Hello, world!\n"))
        .add_code(print_num(501))
        .compile();
}