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
        .add_code(print_str("Hi, world!\\n"))
        .add_code(print_num(4))
        .add_code(print_str("Hello, world!\\n"))
        .add_code(print_num(50000))
        .add_code(print_str("Hi, world!\\n"))
        .compile()
        .run();

    return 0;
}
