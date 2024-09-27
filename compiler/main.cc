#include "core/chunk.h"
#include "core/instruction.h"
#include "core/program.h"
#include "math/binaryoperation.h"
#include "modules/popipo.h"

int main() {
    Program program;

    Variable w("w");
    Variable x("x");
    Variable y("y");
    Variable z("z");

    Chunk chunk1;
    Chunk chunk2;

    program.add_code(chunk1.push_chunk())
        .add_code(w.declare())
        .add_code(x.declare())
        .add_code(y.declare())
        .add_code(z.declare())
        .add_code(chunk1.write_immediate(w, 100))
        .add_code(chunk1.write_immediate(x, -10))
        .add_code(chunk1.write_immediate(y, 20))
        .add_code(chunk1.write_immediate(z, 30))
        .add_code(
            modulo(
                subtract(
                    multiply(chunk1.read_variable(Register::arithmetic_result, w),
                             chunk1.read_variable(Register::arithmetic_result, z)),
                    divide(
                        add(chunk1.read_variable(Register::arithmetic_result, y),
                            chunk1.read_variable(Register::arithmetic_result, y)),
                        chunk1.read_variable(Register::arithmetic_result, x))),
                chunk1.read_variable(Register::arithmetic_result, w)))
        .add_code(print_num(Register::arithmetic_result))
        .add_code(chunk2.push_chunk())
        .add_code(w.declare())
        .add_code(chunk2.write_immediate(w, 105))
        .add_code(chunk2.read_variable(Register::scratch, w))
        .add_code(print_num(Register::scratch))
        .add_code(chunk2.pop_chunk())
        .add_code(chunk1.pop_chunk())
        .compile()
        .run();
}
