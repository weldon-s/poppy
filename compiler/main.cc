#include "control/condition.h"
#include "control/for.h"
#include "control/function.h"
#include "control/if.h"
#include "core/instruction.h"
#include "core/program.h"
#include "math/binaryoperation.h"
#include "memory/chunk.h"
#include "modules/popipo.h"

int main() {
    Program program;
    memory::Variable v{"v"};

    control::Function q{"q", {}, assem::movi(Register::scratch, 10) + popipo::print_num(Register::scratch)};

    control::Function p{"p", {}, assem::movi(Register::scratch, 20) + popipo::print_num(Register::scratch) + q.call({})};

    std::vector<Line> l{};

    l.push_back(assem::movi(Register::scratch, 5));

    program.add_code(q.declare(program) + p.declare(program) + Program::start())
        .add_code(p.call({}))
        .compile()
        .run();
}
