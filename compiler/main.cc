#include "control/condition.h"
#include "control/for.h"
#include "control/if.h"
#include "core/chunk.h"
#include "core/instruction.h"
#include "core/program.h"
#include "math/binaryoperation.h"
#include "modules/popipo.h"

int main() {
    Program program;
    Chunk chunk;
    Variable i{"i"};

    program.add_code(chunk.push_chunk())
        .add_code(control::forloop(
            i.declare() + chunk.write_immediate(i, 1),
            control::le(
                chunk.read_variable(Register::arithmetic_result, i),
                movi(Register::arithmetic_result, 100)),

            add(
                chunk.read_variable(Register::arithmetic_result, i),
                movi(Register::arithmetic_result, 1)) +
                chunk.write_variable(i, Register::arithmetic_result),

            control::ifthenelse(
                control::eq(
                    modulo(
                        chunk.read_variable(Register::arithmetic_result, i),
                        movi(Register::arithmetic_result, 15)),
                    movi(Register::arithmetic_result, 0)),
                print_str("FizzBuzz"),
                control::ifthenelse(
                    control::eq(
                        modulo(
                            chunk.read_variable(Register::arithmetic_result, i),
                            movi(Register::arithmetic_result, 3)),
                        movi(Register::arithmetic_result, 0)),
                    print_str("Fizz"),
                    control::ifthenelse(
                        control::eq(
                            modulo(
                                chunk.read_variable(Register::arithmetic_result, i),
                                movi(Register::arithmetic_result, 5)),
                            movi(Register::arithmetic_result, 0)),
                        print_str("Buzz"),
                        chunk.read_variable(Register::scratch, i) +
                            print_num(Register::scratch)))) +
                print_str("\n")))
        .add_code(chunk.pop_chunk())
        .compile()
        .run();
}
