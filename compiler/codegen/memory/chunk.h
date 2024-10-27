#ifndef CHUNK
#define CHUNK

#include <map>
#include <vector>

#include "codegen/core/instruction.h"
#include "codegen/core/register.h"
#include "codegen/memory/variable.h"
namespace memory {
// represents a chunk allocated on the stack
class Chunk {
    std::map<Variable, int> offsets;
    int _size;
    Chunk* _previous;

   public:
    Chunk(Chunk* previous = nullptr);

    // adds a variable to the chunk
    void add_variable(const Variable& v);
    bool has_variable(const Variable& v) const;

    Line push_chunk();
    static Line pop_chunk();
    int size() const;

    Line read_variable(const Register& reg, const Variable& variable, const Register& address = Register::frame_pointer) const;
    Line write_variable(const Variable& variable, const Register& reg, const Register& address = Register::frame_pointer) const;
    Line write_immediate(const Variable& variable, long long imm) const;

    Chunk* previous() const;
};
}  // namespace memory

#endif
