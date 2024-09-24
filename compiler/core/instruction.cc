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

Line mov(const Register& dest, const Register& src) {
    return Line{new Instruction{std::format("mov {}, {}", dest, src)}};
}

Line movi(const Register& dest, long long imm) {
    // this instruction needs to support 64-bit immediate values
    // ARM only supports 16-bit, so we recursively split the immediate value

    if ((imm <= 65535) && (imm >= -65537)) {
        return Line{new Instruction{std::format("mov {}, #{}", dest, imm)}};
    }

    // split the immediate value into upper and lower parts
    long long upper = imm >> 12;
    long long lower = imm & 0xFFF;

    // create the instructions to move upper
    Line mov_upper{movi(dest, upper)};

    // create the instructions to move lower
    return std::move(mov_upper) +
           Line{
               new Instruction{Instruction{std::format("lsl {}, {}, #12", dest, dest)} +
                               std::format("add {}, {}, #{}", dest, dest, lower)}};
}

Line push(const Register& reg) {
    return Line{new Instruction{std::format("str {}, [sp, #-16]!", reg)}};
}

Line push_pair(const Register& reg1, const Register& reg2) {
    return Line{
        new Instruction{std::format("stp {}, {}, [sp, #-16]!", reg1, reg2)}};
}

Line pop(const Register& reg) {
    return Line{
        new Instruction{
            std::format("ldr {}, [sp], #16", reg)}};
}

Line pop_pair(const Register& reg1, const Register& reg2) {
    return Line{
        new Instruction{
            std::format("ldp {}, {}, [sp], #16", reg1, reg2)}};
}

Line add(const Register& dest, const Register& src1, const Register& src2) {
    return Line{new Instruction{std::format("add {}, {}, {}", dest, src1, src2)}};
}
