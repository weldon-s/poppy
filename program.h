#ifndef PROGRAM_H
#define PROGRAM_H
// represents a Poppy program
#include <iostream>
#include <string>
#include <vector>

class Program {
    // important bits that define _start, end gracefully
    static const std::string head;
    static const std::string tail;

    // dependencies for the program (assembly files without the .s extension)
    std::vector<std::string> includes;
    std::vector<std::string> code;  // the code of the program

   public:
    Program();
    Program& add_include(std::string include);
    Program& add_code(std::string line);
    void compile(std::string name = "out") const;  // compiles the program into an executable

    friend std::ostream& operator<<(std::ostream& os, const Program& program);
};

#endif