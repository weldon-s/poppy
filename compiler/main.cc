#include "control/condition.h"
#include "control/if.h"
#include "control/while.h"
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
        .add_code(i.declare())
        .add_code(chunk.write_immediate(i, 1))
        .add_code(Line{
            new While{
                le(
                    chunk.read_variable(Register::arithmetic_result, i),
                    movi(Register::arithmetic_result, 100)),

                Line{new If{
                    eq(
                        modulo(
                            chunk.read_variable(Register::arithmetic_result, i),
                            movi(Register::arithmetic_result, 15)),
                        movi(Register::arithmetic_result, 0)),
                    print_str("FizzBuzz"),
                    Line{new If{
                        eq(
                            modulo(
                                chunk.read_variable(Register::arithmetic_result, i),
                                movi(Register::arithmetic_result, 3)),
                            movi(Register::arithmetic_result, 0)),
                        print_str("Fizz"),
                        Line{
                            new If{
                                eq(
                                    modulo(
                                        chunk.read_variable(Register::arithmetic_result, i),
                                        movi(Register::arithmetic_result, 5)),
                                    movi(Register::arithmetic_result, 0)),
                                print_str("Buzz"),
                                chunk.read_variable(Register::scratch, i) +
                                    print_num(Register::scratch)}}}}}} +
                    print_str("\n") +
                    add(
                        chunk.read_variable(Register::arithmetic_result, i),
                        movi(Register::arithmetic_result, 1)) +
                    chunk.write_variable(i, Register::arithmetic_result)}})
        .add_code(chunk.pop_chunk())
        .compile()
        .run();
}
