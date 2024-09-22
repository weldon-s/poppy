#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <memory>
#include <string>

#include "core/code.h"

// represents an ARM instruction
// mostly just a wrapper for strings right now, but could be expanded
class Instruction : public Code {
    const std::string assembly;
    std::ostream& stream(std::ostream& os) const override;

    static std::string combine(const std::vector<std::string>& assembly);
    static std::string combine(const std::vector<const char*>& assembly);

   public:
    Instruction(const std::string assembly);
    Instruction(const char* assembly);
    Instruction(std::vector<const char*> assembly);
    Instruction(std::vector<std::string> assembly);
    Instruction operator+(const Instruction& instruction) const;
};

Line mov(int dest, int src);         // move from src to dest
Line movi(int dest, long long imm);  // move immediate value to dest (supports 64-bit values)
Line push(int reg);                  // push register onto stack
Line pop(int reg);                   // pop register from stack

#endif