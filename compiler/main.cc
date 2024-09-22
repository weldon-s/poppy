#include "core/chunk.h"
#include "core/instruction.h"
#include "core/program.h"
#include "modules/popipo.h"

int main() {
    Program program;

    Variable x("x");
    Variable y("y");

    Chunk chunk1({x, y});
    Chunk chunk2({x, y});

    program
        .add_include("print_num")
        .add_code(chunk1.push_chunk())
        .add_code(movi(9, 400))
        .add_code(chunk1.write_variable(x, 9))
        .add_code(movi(9, 500))
        .add_code(chunk1.write_variable(y, 9))
        .add_code(chunk2.push_chunk())
        .add_code(movi(9, 123456789098599595))
        .add_code(chunk2.write_variable(x, 9))
        .add_code(movi(9, 200))
        .add_code(chunk2.write_variable(y, 9))
        .add_code(chunk2.read_variable(9, x))
        .add_code(print_num(9))
        .add_code(chunk2.read_variable(9, y))
        .add_code(print_num(9))
        .add_code(chunk2.pop_chunk())
        .add_code(chunk1.read_variable(9, x))
        .add_code(print_num(9))
        .add_code(chunk1.read_variable(9, y))
        .add_code(print_num(9))
        .add_code(chunk1.pop_chunk())
        .compile()
        .run();

    // program
    //     .add_include("print_num")
    //     .add_code(print_str("Hi, world!\\n"))
    //     .add_code(movi(9, 4))
    //     .add_code(print_num(9))
    //     .add_code(print_str("Hello, world!\\n"))
    //     .add_code(movi(9, 50000))
    //     .add_code(print_num(9))
    //     .add_code(print_str("Hi, world!\\n"))
    //     .compile()
    //     .run();

    return 0;
}
