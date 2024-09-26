#include "core/chunk.h"

#include <format>

// make sure size is multiple of 16 bytes
Chunk::Chunk(std::vector<Variable> variables) : size{8 * (variables.size() + 1 + ((variables.size() + 1) % 2))} {
    int offset{8};

    for (const Variable& variable : variables) {
        offsets[variable] = offset;
        offset += 8;
    }

    if (offsets.size() != variables.size()) {
        throw std::runtime_error("Duplicate variable name in chunk");
    }
}

Line Chunk::push_chunk() const {
    // subtract the size from the stack pointer
    // store the size at the top of the chunk
    return movi(Register::scratch, size) +
           Line(new Instruction(Instruction("sub sp, sp, x9") + "str x9, [sp]")) +
           mov(Register::frame_pointer, Register::stack_pointer);
}

Line Chunk::pop_chunk() const {
    // add the size back to the stack pointer
    return Line(new Instruction(Instruction("ldr x9, [sp]") + "add sp, sp, x9")) +
           mov(Register::frame_pointer, Register::stack_pointer);
}

Line Chunk::read_variable(const Register& reg, const Variable& variable) const {
    // read the variable from the stack
    return Line(new Instruction(Instruction(std::format("ldr {}, [{}, #{}]", reg, Register::frame_pointer, offsets.at(variable)))));
}

Line Chunk::write_variable(const Variable& variable, const Register& reg) const {
    // write the variable to the stack
    return Line(new Instruction(Instruction(std::format("str {}, [{}, #{}]", reg, Register::frame_pointer, offsets.at(variable)))));
}

Line Chunk::write_immediate(const Variable& variable, long long imm) const {
    return movi(Register::scratch, imm) +
           write_variable(variable, Register::scratch);
}