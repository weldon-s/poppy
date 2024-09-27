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
    int _size;

   public:
    Chunk();

    // adds a variable to the chunk
    void add_variable(const Variable& v);

    Line push_chunk();
    Line pop_chunk();
    int size() const;

    Line read_variable(const Register& reg, const Variable& variable) const;
    Line write_variable(const Variable& variable, const Register& reg) const;
    Line write_immediate(const Variable& variable, long long imm) const;
};

#endif
