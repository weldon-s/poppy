#include "core/chunk.h"

#include <format>

Variable::Variable(std::string name) : name(name) {}

bool Variable::operator==(const Variable& other) const {
    return this == &other;
}

bool Variable::operator<(const Variable& other) const {
    return name < other.name;
}

// make sure size is multiple of 16 bytes
Chunk::Chunk(std::vector<Variable> variables) : size{8 * (variables.size() + 1 + ((variables.size() + 1) % 2))} {
    int offset{8};

    for (const Variable& variable : variables) {
        offsets[variable] = offset;
        offset += 8;
    }
}

Line Chunk::push_chunk() const {
    // subtract the size from the stack pointer
    // store the size at the top of the chunk
    return movi(scratch, size) + Line(new Instruction(Instruction("sub sp, sp, x9") + "str x9, [sp]"));
}

Line Chunk::pop_chunk() const {
    // add the size back to the stack pointer
    return Line(new Instruction(Instruction("ldr x9, [sp]") + "add sp, sp, x9"));
}

Line Chunk::read_variable(const Register& reg, const Variable& variable) const {
    // read the variable from the stack
    // precondition: sp points to top of stack
    return Line(new Instruction(Instruction(std::format("ldr x{}, [sp, #{}]", reg.reg, offsets.at(variable)))));
}

Line Chunk::write_variable(const Variable& variable, const Register& reg) const {
    // write the variable to the stack
    // precondition: sp points to top of stack
    return Line(new Instruction(Instruction(std::format("str x{}, [sp, #{}]", reg.reg, offsets.at(variable)))));
}