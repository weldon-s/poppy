#include "program.h"

int main() {
    Program program;

    program.add_include("print_num")
        .add_code("mov x1, #4")
        .add_code("bl print_num");

    program.compile();
}