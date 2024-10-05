#include "control/condition.h"
#include "control/for.h"
#include "control/if.h"
#include "core/instruction.h"
#include "core/program.h"
#include "math/binaryoperation.h"
#include "memory/chunk.h"
#include "modules/popipo.h"

int main() {
    Program program;
    memory::Chunk chunk;
    memory::Variable i{"i"};

    program.add_code(chunk.push_chunk())
        .add_code(control::forloop(
            i.declare() + chunk.write_immediate(i, 1),
            control::le(
                chunk.read_variable(Register::arithmetic_result, i),
                movi(Register::arithmetic_result, 100)),

            math::add(
                chunk.read_variable(Register::arithmetic_result, i),
                movi(Register::arithmetic_result, 1)) +
                chunk.write_variable(i, Register::arithmetic_result),

            control::ifthenelse(
                control::eq(
                    math::modulo(
                        chunk.read_variable(Register::arithmetic_result, i),
                        movi(Register::arithmetic_result, 15)),
                    movi(Register::arithmetic_result, 0)),
                popipo::print_str("FizzBuzz"),
                control::ifthenelse(
                    control::eq(
                        math::modulo(
                            chunk.read_variable(Register::arithmetic_result, i),
                            movi(Register::arithmetic_result, 3)),
                        movi(Register::arithmetic_result, 0)),
                    popipo::print_str("Fizz"),
                    control::ifthenelse(
                        control::eq(
                            math::modulo(
                                chunk.read_variable(Register::arithmetic_result, i),
                                movi(Register::arithmetic_result, 5)),
                            movi(Register::arithmetic_result, 0)),
                        popipo::print_str("Buzz"),
                        chunk.read_variable(Register::scratch, i) +
                            popipo::print_num(Register::scratch)))) +
                popipo::print_str("\n")))
        .add_code(chunk.pop_chunk())
        .compile()
        .run();
}
