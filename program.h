#ifndef PROGRAM_H
#define PROGRAM_H
// represents a Poppy program
#include <iostream>
#include <string>
#include <vector>

#include "instruction.h"

class Program {
    // assembly path
    static const std::string assembly_path;

    // important bits that define _start, end gracefully
    static const std::string head;
    static const std::string tail;

    // dependencies for the program (assembly files without the .s extension)
    std::vector<std::string> includes;
    std::vector<Instruction> code;  // the code of the program

   public:
    Program();
    Program& add_include(std::string include);
    Program& add_code(Instruction line);

    void compile(std::string name) const;  // compiles the program into an executable
    void compile() const;

    friend std::ostream& operator<<(std::ostream& os, const Program& program);
};

#endif