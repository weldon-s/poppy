#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <string>

// represents an ARM instruction
// mostly just a wrapper for strings right now, but could be expanded
class Instruction {
    std::string assembly;

   public:
    Instruction(std::string assembly);
    Instruction(const char* assembly);
    Instruction operator+(const Instruction& instruction) const;

    friend std::ostream& operator<<(std::ostream& os, const Instruction& instruction);
};

Instruction mov(int dest, int src);
Instruction movi(int dest, long long imm);
#endif