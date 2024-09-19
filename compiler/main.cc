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

    Instruction* i1{new Instruction{print_str("hi", "Hi, world!\\n")}};
    Instruction* i2{new Instruction{print_num(4)}};
    Instruction* i3{new Instruction{print_str("hello", "Hello, world!\\n")}};
    Instruction* i4{new Instruction{print_num(50000)}};

    program.add_include("print_num")
        .add_code(std::unique_ptr<Code>(i1))
        .add_code(std::unique_ptr<Code>(i2))
        .add_code(std::unique_ptr<Code>(i3))
        .add_code(std::unique_ptr<Code>(i4))
        .compile()
        .run();

    return 0;
}
