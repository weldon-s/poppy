// #include "core/chunk.h"
#include "core/instruction.h"
#include "core/program.h"
#include "modules/popipo.h"

int main() {
    Program program;

    // Variable x("x");
    // Variable y("y");

    // Chunk chunk({x, y});

    // for (auto [variable, offset] : chunk.offsets) {
    //     std::cout << variable.name << " " << offset << std::endl;
    // }

    program.add_include("print_num")
        .add_code(print_str("hi", "Hi, world!\\n"))
        .add_code(print_num(4))
        .add_code(print_str("hello", "Hello, world!\\n"))
        .add_code(print_num(501))
        .compile();

    return 0;
}
