#include "control/if.h"
#include "control/label.h"
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

    Label* label = program.get_label();
    std::string label_name = label->label();

    program.add_code(chunk.push_chunk())
        .add_code(x.declare())
        .add_code(y.declare())
        .add_code(chunk.write_immediate(x, 0))
        .add_code(chunk.write_immediate(y, -22220))

        .add_code(Line{
            new If{
                chunk.read_variable(Register::arithmetic_result, x),
                chunk.read_variable(Register::scratch, y) +
                    print_num(Register::scratch),
                chunk.read_variable(Register::scratch, x) +
                    print_num(Register::scratch)}})
        .add_code(chunk.pop_chunk())
        .compile()
        .run();
}
