#include "instruction.h"
#include "modules/popipo.h"
#include "program.h"

int main() {
    Program program;

    program.add_include("print_num")
        .add_code(print_str("str", "Hello I am a string\\n"))
        .add_code(print_num(42))
        .add_code(print_str("str2", "Goodbye I am another string\\n"));
    program.compile();
}