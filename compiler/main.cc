#include "control/condition.h"
#include "control/for.h"
#include "control/function.h"
#include "control/if.h"
#include "core/instruction.h"
#include "core/program.h"
#include "math/binaryoperation.h"
#include "memory/chunk.h"
#include "modules/popipo.h"

int main(int argc, char* argv[]) {
    Program program;
    memory::Variable v{"v"};

    control::Function p{"p", {v}};

    std::vector<Line> args1{};
    std::vector<Line> args2{};
    args1.push_back(math::subtract(p.read_variable(Register::arithmetic_result, v),
                                   assem::movi(Register::arithmetic_result, 1)));

    args2.push_back(math::subtract(p.read_variable(Register::arithmetic_result, v),
                                   assem::movi(Register::arithmetic_result, 2)));

    p.set_body(
        control::ifthenelse(
            control::le(
                p.read_variable(Register::arithmetic_result, v),
                assem::movi(Register::arithmetic_result, 1)),
            p.read_variable(Register::arithmetic_result, v),
            math::add(
                p.call(args1), p.call(args2))));

    std::vector<Line> l{};
    l.push_back(assem::movi(Register::arithmetic_result, 20));

    program.add_code(p.declare(program))
        .add_code(Program::start())
        .add_code(p.call(l))
        .add_code(popipo::print_num(Register::arithmetic_result))
        .compile()
        .run();
}
