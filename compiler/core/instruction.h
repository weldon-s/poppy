#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <memory>
#include <string>

#include "core/code.h"
#include "core/register.h"

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

    Line simplify(Program& program) override;
};

Line mov(const Register& dest, const Register& src);                         // move from src to dest
Line movi(const Register& dest, long long imm);                              // move immediate value to dest (supports 64-bit values)
Line push(const Register& reg);                                              // push register onto stack
Line push_pair(const Register& reg1, const Register& reg2);                  // push two registers onto stack
Line pop(const Register& reg);                                               // pop register from stack
Line pop_pair(const Register& reg1, const Register& reg2);                   // pop two registers from stack
Line add(const Register& dest, const Register& src1, const Register& src2);  // add src1 and src2, store in dest
#endif