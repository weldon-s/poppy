#include "control/function.h"

#include <cassert>

#include "core/instruction.h"
#include "core/program.h"

namespace control {
Function::Function(std::vector<memory::Variable> arguments, Line body) : arguments{arguments}, body{std::move(body)} {}

Line Function::declare(Program& program) {
    start_label.emplace(program.get_label());
    memory::Variable caller_frame_pointer{"caller_frame_pointer"};  // dynamicLink
    memory::Variable saved_link{"saved_link"};                      // savedPC
    memory::Variable arg_chunk_pointer{"arg_chunk_pointer"};        // paramPtr

    memory::Chunk& param_chunk = program.top_chunk();
    memory::Chunk chunk{&param_chunk};

    return start_label->declare() +
           assem::mov(Register::arg_chunk_pointer, Register::stack_pointer) +  // was result instead of sp
           chunk.push_chunk() +
           caller_frame_pointer.declare() +
           saved_link.declare() +
           arg_chunk_pointer.declare() +
           chunk.write_variable(caller_frame_pointer, Register::frame_pointer) +
           assem::mov(Register::frame_pointer, Register::stack_pointer) +  // was result instead of sp
           chunk.write_variable(saved_link, Register::link_register) +
           chunk.write_variable(arg_chunk_pointer, Register::arg_chunk_pointer) +
           std::move(body) +
           chunk.read_variable(Register::link_register, saved_link) +
           chunk.read_variable(Register::frame_pointer, caller_frame_pointer) +
           memory::Chunk::pop_chunk() +
           memory::Chunk::pop_chunk() +
           ret();
}

Line Function::call(std::vector<Line> to_evaluate) const {
    memory::Chunk param_chunk;
    Line declarations;

    assert(to_evaluate.size() == arguments.size() && "Number of arguments does not match");

    for (size_t i{0}; i < arguments.size(); ++i) {
        declarations = std::move(declarations) +
                       arguments[i].declare() +
                       std::move(to_evaluate[i]) +
                       param_chunk.write_variable(arguments[i], Register::arithmetic_result);
    }

    return param_chunk.push_chunk() +
           std::move(declarations) +
           start_label->bl();
}

Line ret() {
    return Line(new Instruction{"ret"});
}
}  // namespace control