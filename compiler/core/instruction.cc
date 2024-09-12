#include "core/instruction.h"

#include <format>

Instruction::Instruction(const std::string& assembly) : assembly(assembly) {}
Instruction::Instruction(const char* assembly) : assembly(assembly) {}

Instruction Instruction::operator+(const Instruction& instruction) const {
    return assembly + "\n" + instruction.assembly;
}

std::ostream& operator<<(std::ostream& os, const Instruction& instruction) {
    os << instruction.assembly;
    return os;
}

Instruction mov(int dest, int src) {
    return std::format("mov x{}, x{}", dest, src);
}

Instruction movi(int dest, long long imm) {
    // this instruction needs to support 64-bit immediate values
    // ARM only supports 16-bit, so we recursively split the immediate value

    if ((imm <= 65535) && (imm >= -65537)) {
        return std::format("mov x{}, #{}", dest, imm);
    }

    // split the immediate value into upper and lower parts
    long long upper = imm >> 12;
    long long lower = imm & 0xFFF;

    // create the instructions to move upper
    Instruction mov_upper{movi(dest, upper)};

    // create the instructions to move lower
    return mov_upper +
           std::format("lsl x{}, x{}, #12", dest, dest) +
           std::format("add x{}, x{}, #{}", dest, dest, lower);
}

Instruction push(int reg) {
    return std::format("str x{}, [sp, #-8]!", reg);
}

Instruction pop(int reg) {
    return std::format("ldr x{}, [sp], #8", reg);
}
