#include "core/chunk.h"

#include <format>
#include <functional>

#include "core/program.h"

Chunk::Chunk() : _size{16} {}

void Chunk::add_variable(const Variable& v) {
    int old_count{offsets.size()};
    offsets[v] = (old_count + 1) * 8;

    int new_count{offsets.size()};

    // if new_count is even, we have to use an odd number of words
    // -> have to increment by 16 to keep the stack pointer aligned
    if (new_count % 2 == 0) {
        _size += 16;
    }
}

int Chunk::size() const {
    return _size;
}
class PushChunkCode : public Code {
    Chunk* chunk;

   public:
    PushChunkCode(Chunk* chunk) : Code{true}, chunk{chunk} {}

    std::ostream& stream(std::ostream& os) const override {
        return os;
    }

    Line simplify(Program& program) override {
        // subtract the size from the stack pointer
        // store the size at the top of the chunk
        return movi(Register::scratch, chunk->size()) +
               Line(new Instruction(Instruction("sub sp, sp, x9") + "str x9, [sp]")) +
               mov(Register::frame_pointer, Register::stack_pointer);
    }

    void allocate(Program& program) override {
        program.push_chunk(chunk);
    }
};

class PopChunkCode : public Code {
    Chunk* chunk;

   public:
    PopChunkCode(Chunk* chunk) : Code{true}, chunk{chunk} {}

    std::ostream& stream(std::ostream& os) const override {
        return os;
    }

    Line simplify(Program& program) override {
        // add the size back to the stack pointer
        return Line(new Instruction(Instruction("ldr x9, [sp]") + "add sp, sp, x9")) +
               mov(Register::frame_pointer, Register::stack_pointer);
    }

    void allocate(Program& program) override {
        program.pop_chunk();
    }
};

Line Chunk::push_chunk() {
    return Line(new PushChunkCode(this));
}

Line Chunk::pop_chunk() {
    return Line(new PopChunkCode(this));
}

/*  reads/writes need to be evaluated after allocation as the offset can't be known before we allocate the variables
    doing it earlier would require us passing in a program since we don't know if child codes contain variables to be
    allocated for the current chunk
*/

class LazilyEvaluatedCode : public Code {
    const Chunk* chunk;
    const std::function<Line(const Chunk*)> evaluator;

   public:
    LazilyEvaluatedCode(const Chunk* chunk, const std::function<Line(const Chunk*)> evaluator)
        : Code{true}, chunk{chunk}, evaluator{evaluator} {}

    std::ostream& stream(std::ostream& os) const override {
        return os;
    }

    Line simplify(Program& program) override {
        return evaluator(chunk);
    }
};

Line Chunk::read_variable(const Register& reg, const Variable& variable) const {
    auto evaluator = [reg, variable](const Chunk* chunk) {
        // read the variable from the stack
        return Line(new Instruction(std::format("ldr {}, [{}, #{}]", reg, Register::frame_pointer, chunk->offsets.at(variable))));
    };

    return Line{new LazilyEvaluatedCode{this, evaluator}};
}

Line Chunk::write_variable(const Variable& variable, const Register& reg) const {
    auto evaluator = [variable, reg](const Chunk* chunk) {
        // write the variable to the stack
        return Line(new Instruction(std::format("str {}, [{}, #{}]", reg, Register::frame_pointer, chunk->offsets.at(variable))));
    };

    return Line{new LazilyEvaluatedCode{this, evaluator}};
}

Line Chunk::write_immediate(const Variable& variable, long long imm) const {
    return movi(Register::scratch, imm) +
           write_variable(variable, Register::scratch);
}