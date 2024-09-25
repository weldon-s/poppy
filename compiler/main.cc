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
        .add_code(print_str("Hello, worl\"d!\n") + print_str("Hello, world!\n") + print_str("Hello, world!\n") +
                  (chunk1.push_chunk() +
                   (movi(Register::scratch, 400) + chunk1.write_variable(x, Register::scratch)) +
                   (movi(Register::scratch, 500) + chunk1.write_variable(y, Register::scratch))) +
                  chunk2.push_chunk() +
                  movi(Register::scratch, 123456789098599595) +
                  chunk2.write_variable(x, Register::scratch) +
                  movi(Register::scratch, 200) +
                  chunk2.write_variable(y, Register::scratch) +
                  chunk2.read_variable(Register::scratch, x) +
                  print_num(Register::scratch) +
                  chunk2.read_variable(Register::scratch, y) +
                  print_num(Register::scratch) +
                  chunk2.pop_chunk() +
                  chunk1.read_variable(Register::scratch, x) +
                  print_num(Register::scratch) +
                  chunk1.read_variable(Register::scratch, y) +
                  print_num(Register::scratch) +
                  chunk1.pop_chunk())
        .compile()
        .run();

    return 0;
}
