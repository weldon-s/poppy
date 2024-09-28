#include "control/condition.h"
#include "control/if.h"
#include "core/chunk.h"
#include "core/instruction.h"
#include "core/program.h"
#include "math/binaryoperation.h"
#include "modules/popipo.h"

int main() {
    Program program;

    Variable x("x");
    Variable y("y");

    Chunk chunk;

    program.add_code(chunk.push_chunk())
        .add_code(x.declare())
        .add_code(y.declare())
        .add_code(chunk.write_immediate(x, -1110))
        .add_code(chunk.write_immediate(y, -50))

        .add_code(Line{
            new If{
                lt(
                    chunk.read_variable(Register::arithmetic_result, x),
                    chunk.read_variable(Register::arithmetic_result, y)),
                movi(Register::scratch, 1111) +
                    print_num(Register::scratch),
                movi(Register::scratch, 0) +
                    print_num(Register::scratch)}})
        .add_code(chunk.pop_chunk())
        .compile()
        .run();
}
