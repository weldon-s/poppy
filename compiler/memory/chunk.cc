#include "memory/chunk.h"

#include <format>
#include <functional>

#include "core/program.h"
namespace memory {

Chunk::Chunk(Chunk* previous) : _size{16}, _previous{previous} {}

void Chunk::add_variable(const Variable& v) {
    size_t old_count{offsets.size()};
    offsets[v] = (old_count + 1) * 8;

    size_t new_count{offsets.size()};

    // if new_count is even, we have to use an odd number of words
    // -> have to increment by 16 to keep the stack pointer aligned
    if (new_count % 2 == 0) {
        _size += 16;
    }
}

bool Chunk::has_variable(const Variable& v) const {
    return offsets.find(v) != offsets.end();
}

int Chunk::size() const {
    return _size;
}

Chunk* Chunk::previous() const {
    return _previous;
}

class PushChunkCode : public Code {
    Chunk* chunk;

   public:
    PushChunkCode(Chunk* chunk) : Code{false}, chunk{chunk} {}

    Line simplify(Program& program) override {
        // subtract the size from the stack pointer
        // store the size at the top of the chunk

        program.push_chunk(chunk);

        return assem::movi(Register::scratch, chunk->size()) +
               Line(new Instruction(Instruction("sub sp, sp, x9") + "str x9, [sp]"));
    }

    void allocate(Program& program) override {
        program.push_chunk(chunk);
    }
};

class PopChunkCode : public Code {
   public:
    PopChunkCode() : Code{false} {}

    Line simplify(Program& program) override {
        program.pop_chunk();

        // add the size back to the stack pointer
        return Line(new Instruction(Instruction("ldr x9, [sp]") + "add sp, sp, x9"));
    }

    void allocate(Program& program) override {
        program.pop_chunk();
    }
};

Line Chunk::push_chunk() {
    return Line(new PushChunkCode(this));
}

Line Chunk::pop_chunk() {
    return Line(new PopChunkCode());
}

/*  reads/writes need to be evaluated after allocation as the offset can't be known before we allocate the variables
    doing it earlier would require us passing in a program since we don't know if child codes contain variables to be
    allocated for the current chunk
*/

Line Chunk::read_variable(const Register& reg, const Variable& variable, const Register& address) const {
    auto evaluator = [reg, variable, address, this]() {
        // read the variable from the stack
        return Line(new Instruction(std::format("ldr {}, [{}, #{}]", reg, address, offsets.at(variable))));
    };

    return lazy(evaluator);
}

Line Chunk::write_variable(const Variable& variable, const Register& reg, const Register& address) const {
    auto evaluator = [variable, reg, address, this]() {
        // write the variable to the stack
        return Line(new Instruction(std::format("str {}, [{}, #{}]", reg, address, offsets.at(variable))));
    };

    return lazy(evaluator);
}

Line Chunk::write_immediate(const Variable& variable, long long imm) const {
    return assem::movi(Register::scratch, imm) +
           write_variable(variable, Register::scratch);
}
}  // namespace memory
