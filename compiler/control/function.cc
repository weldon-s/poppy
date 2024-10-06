#include "control/function.h"

#include <cassert>

#include "core/instruction.h"
#include "core/program.h"

namespace control {
Function::Function(const std::string name, std::vector<memory::Variable> arguments, Line body)
    : arguments{arguments},
      body{std::move(body)},
      start_label{control::Label{name}},
      caller_frame_pointer{std::make_unique<memory::Variable>("caller_frame_pointer")},
      saved_link{std::make_unique<memory::Variable>("saved_link")},
      arg_chunk_pointer{std::make_unique<memory::Variable>("arg_chunk_pointer")},
      frame{std::make_unique<memory::Chunk>()},
      param_chunk{std::make_unique<memory::Chunk>()} {
    for (const memory::Variable& arg : arguments) {
        param_chunk->add_variable(arg);
    }
}

Line Function::declare(Program& program) {
    // memory::Chunk& param_chunk = program.top_chunk();
    // memory::Chunk chunk{&param_chunk};

    return start_label.declare() +
           assem::mov(Register::arg_chunk_pointer, Register::stack_pointer) +  // was result instead of sp
           frame->push_chunk() +
           caller_frame_pointer->declare() +
           saved_link->declare() +
           arg_chunk_pointer->declare() +
           frame->write_variable(*caller_frame_pointer, Register::frame_pointer) +
           assem::mov(Register::frame_pointer, Register::stack_pointer) +  // was result instead of sp
           frame->write_variable(*saved_link, Register::link_register) +
           frame->write_variable(*arg_chunk_pointer, Register::arg_chunk_pointer) +
           std::move(body) +
           frame->read_variable(Register::link_register, *saved_link) +
           frame->read_variable(Register::frame_pointer, *caller_frame_pointer) +
           memory::Chunk::pop_chunk() +
           ret();
}

Line Function::call(std::vector<Line> to_evaluate) const {
    assert(to_evaluate.size() == arguments.size() && "Number of arguments does not match");

    Line declarations = Line{new Instruction{""}};

    for (size_t i{0}; i < arguments.size(); ++i) {
        declarations = std::move(declarations) +
                       std::move(to_evaluate[i]) +
                       param_chunk->write_variable(arguments[i], Register::arithmetic_result);
    }

    return param_chunk->push_chunk() +
           std::move(declarations) +
           start_label.bl() +
           param_chunk->pop_chunk();
}

Line ret() {
    return Line(new Instruction{"ret"});
}
}  // namespace control