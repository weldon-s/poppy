#include "instruction.h"
#include "program.h"

int main() {
    Program program;

    program.add_include("print_num")
        .add_code(movi(1, -123456789))
        .add_code("bl print_num");

    program.compile();
}