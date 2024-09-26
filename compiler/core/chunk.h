#ifndef CHUNK
#define CHUNK

#include <map>
#include <vector>

#include "core/variable.h"
#include "instruction.h"
#include "register.h"

// represents a chunk allocated on the stack
class Chunk {
    std::map<Variable, int> offsets;
    int size;

   public:
    Chunk(std::vector<Variable> variables);

    // TODO ensure sp is aligned to 16 bytes
    Line push_chunk() const;
    Line pop_chunk() const;
    Line read_variable(const Register& reg, const Variable& variable) const;
    Line write_variable(const Variable& variable, const Register& reg) const;
    Line write_immediate(const Variable& variable, long long imm) const;
};

#endif