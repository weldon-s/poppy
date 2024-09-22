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
        .add_code(movi(scratch, 400))
        .add_code(chunk1.write_variable(x, scratch))
        .add_code(movi(scratch, 500))
        .add_code(chunk1.write_variable(y, scratch))
        .add_code(chunk2.push_chunk())
        .add_code(movi(scratch, 123456789098599595))
        .add_code(chunk2.write_variable(x, scratch))
        .add_code(movi(scratch, 200))
        .add_code(chunk2.write_variable(y, scratch))
        .add_code(chunk2.read_variable(scratch, x))
        .add_code(print_num(scratch))
        .add_code(chunk2.read_variable(scratch, y))
        .add_code(print_num(scratch))
        .add_code(chunk2.pop_chunk())
        .add_code(chunk1.read_variable(scratch, x))
        .add_code(print_num(scratch))
        .add_code(chunk1.read_variable(scratch, y))
        .add_code(print_num(scratch))
        .add_code(chunk1.pop_chunk())
        .compile()
        .run();

    return 0;
}
