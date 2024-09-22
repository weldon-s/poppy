#include "core/instruction.h"

#include <format>

Instruction::Instruction(const std::string assembly) : Code{true}, assembly(assembly) {}
Instruction::Instruction(const char* assembly) : Code{true}, assembly(assembly) {}
Instruction::Instruction(std::vector<std::string> assembly) : Code{true}, assembly(combine(assembly)) {}
Instruction::Instruction(std::vector<const char*> assembly) : Code{true}, assembly(combine(assembly)) {}

std::string Instruction::combine(const std::vector<std::string>& assembly) {
    std::string result{};

    for (const std::string& line : assembly) {
        result += line + "\n";
    }

    return result;
}

std::string Instruction::combine(const std::vector<const char*>& assembly) {
    std::string result{};

    for (const char* line : assembly) {
        result += line + '\n';
    }

    return result;
}

Instruction Instruction::operator+(const Instruction& instruction) const {
    return assembly + "\n" + instruction.assembly;
}

std::ostream& Instruction::stream(std::ostream& os) const {
    os << assembly;
    return os;
}

Line mov(int dest, int src) {
    return Line{new Instruction{std::format("mov x{}, x{}", dest, src)}};
}

Line movi(int dest, long long imm) {
    // this instruction needs to support 64-bit immediate values
    // ARM only supports 16-bit, so we recursively split the immediate value

    if ((imm <= 65535) && (imm >= -65537)) {
        return Line{new Instruction{std::format("mov x{}, #{}", dest, imm)}};
    }

    // split the immediate value into upper and lower parts
    long long upper = imm >> 12;
    long long lower = imm & 0xFFF;

    // create the instructions to move upper
    Line mov_upper{movi(dest, upper)};

    // create the instructions to move lower
    return std::move(mov_upper) +
           Line{new Instruction{std::vector<std::string>{std::format("lsl x{}, x{}, #12", dest, dest),
                                                         std::format("add x{}, x{}, #{}", dest, dest, lower)}}};
}

Line push(int reg) {
    return Line{new Instruction{std::format("str x{}, [sp, #-8]!", reg)}};
}

Line pop(int reg) {
    return Line{
        new Instruction{
            std::format("ldr x{}, [sp], #8", reg)}};
}
