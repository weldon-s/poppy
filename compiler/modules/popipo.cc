#include "modules/popipo.h"

#include <format>

Instruction print_str(std::string name, std::string str) {
    // unlike numbers, we store strings as data
    // this is generally better since we don't have to worry about the length of the string

    return push(0) + push(1) + push(2) + push(8) +        // preserve values of x0, x1, x2, x8
           ".data" +                                      // switch to data section
           std::format("{}: .ascii \"{}\"", name, str) +  // store the string
           std::format("{}len= . - {}", name, name) +     // calculate the length of the string
           ".text" +                                      // switch back to text section
           movi(0, 1) +                                   // set up syscall arguments (file descriptor = 1, stdout)
           std::format("ldr x1, ={}", name) +             // set up and call syscall
           std::format("ldr x2, ={}len", name) +
           "mov x8, #64" +
           "svc #0" +
           pop(8) + pop(2) + pop(1) + pop(0);  // restore values of x0, x1, x2, x8
}

Instruction print_num(long long num) {
    return push(1) + movi(1, num) + "bl print_num" + pop(1);
}