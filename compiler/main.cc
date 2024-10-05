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
    control::Function p{{}, assem::movi(Register::scratch, 10) + popipo::print_num(Register::scratch)};

    program.add_code(p.declare(program) + p.call({}))
        .compile()
        .run();
}
