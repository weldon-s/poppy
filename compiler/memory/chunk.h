#ifndef CHUNK
#define CHUNK

#include <map>
#include <vector>

#include "core/instruction.h"
#include "core/register.h"
#include "memory/variable.h"
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

    Line push_chunk();
    Line pop_chunk();
    int size() const;

    Line read_variable(const Register& reg, const Variable& variable) const;
    Line write_variable(const Variable& variable, const Register& reg) const;
    Line write_immediate(const Variable& variable, long long imm) const;

    Chunk* previous() const;
    void set_previous(Chunk* previous);
};
}  // namespace memory

#endif
