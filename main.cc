#include "program.h"

int main() {
    Program program;

    program.add_include("print_num");
    program.add_code("mov x1, #412");
    program.add_code("bl print_num");

    program.compile("out");
}