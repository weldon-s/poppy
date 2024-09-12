#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <string>

// represents an ARM instruction
// mostly just a wrapper for strings right now, but could be expanded
class Instruction {
    const std::string& assembly;

   public:
    Instruction(const std::string& assembly);
    Instruction(const char* assembly);
    Instruction operator+(const Instruction& instruction) const;

    friend std::ostream& operator<<(std::ostream& os, const Instruction& instruction);
};

Instruction mov(int dest, int src);         // move from src to dest
Instruction movi(int dest, long long imm);  // move immediate value to dest (supports 64-bit values)
Instruction push(int reg);                  // push register onto stack
Instruction pop(int reg);                   // pop register from stack

#endif