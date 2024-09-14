#ifndef PROGRAM_H
#define PROGRAM_H
// represents a Poppy program
#include <iostream>
#include <string>
#include <vector>

#include "core/instruction.h"
#include "core/transformer.h"

class Code;

class Program {
    // assembly path
    static const std::string assembly_path;

    // important bits that define _start, end gracefully
    static const std::string head;
    static const std::string tail;

    // dependencies for the program (assembly files without the .s extension)
    std::vector<std::string> includes;
    std::vector<const Code*> code;  // the code of the program

    // transformers that will be applied to the code IN THE ORDER THEY APPEAR HERE
    std::vector<Transformer*> transformers;

    void _apply_transformers();

   public:
    Program();
    Program& add_include(const std::string& include);
    Program& add_code(const Code* line);

    void compile(const std::string& name) const;  // compiles the program into an executable
    void compile() const;

    friend std::ostream& operator<<(std::ostream& os, const Program& program);
};

#endif