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

    Instruction i1{print_str("hi", "Hi, world!\\n")};
    Instruction i2{print_num(4)};
    Instruction i3{print_str("hello", "Hello, world!\\n")};
    Instruction i4{print_num(501)};

    program.add_include("print_num")
        .add_code(&i1)
        .add_code(&i2)
        .add_code(&i3)
        .add_code(&i4)
        .compile();

    return 0;
}
