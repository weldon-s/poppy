#include "codegen/control/function.h"

#include <cassert>

#include "codegen/core/instruction.h"
#include "codegen/core/program.h"

namespace control {
Line ret() {
    return Line(new Instruction{"ret"});
}

Function::Function(const std::string name, std::vector<memory::Variable> arguments)
    : arguments{arguments},
      start_label{control::Label{"function_" + name}},
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
    return start_label.declare() +
           assem::mov(Register::arg_chunk_pointer, Register::stack_pointer) +
           frame->push_chunk() +
           caller_frame_pointer->declare() +
           saved_link->declare() +
           arg_chunk_pointer->declare() +
           frame->write_variable(*caller_frame_pointer, Register::frame_pointer, Register::stack_pointer) +
           assem::mov(Register::frame_pointer, Register::stack_pointer) +
           frame->write_variable(*saved_link, Register::link_register, Register::stack_pointer) +
           frame->write_variable(*arg_chunk_pointer, Register::arg_chunk_pointer, Register::stack_pointer) +
           std::move(body) +
           frame->read_variable(Register::link_register, *saved_link, Register::stack_pointer) +
           frame->read_variable(Register::frame_pointer, *caller_frame_pointer, Register::stack_pointer) +
           memory::Chunk::pop_chunk() +
           ret();
}

Line Function::call(std::vector<Line>& to_evaluate) const {
    auto evaluator = [this, &to_evaluate]() {
        assert(to_evaluate.size() == arguments.size() && "Number of arguments does not match");

        Line declarations = Line{new Instruction{""}};

        for (size_t i{0}; i < arguments.size(); ++i) {
            declarations = std::move(declarations) +
                           std::move(to_evaluate[i]) +
                           param_chunk->write_variable(arguments[i], Register::arithmetic_result, Register::stack_pointer);
        }

        return param_chunk->push_chunk() +
               std::move(declarations) +
               start_label.bl() +
               param_chunk->pop_chunk();
    };

    return lazy(evaluator);
}

Line Function::read_variable(const Register& reg, const memory::Variable& variable) const {
    auto evaluator = [reg, variable, this](Program& program) {
        if (frame->has_variable(variable)) {
            // frame is on top of the stack (otherwise we wouldn't be in this function)
            return frame->read_variable(reg, variable);
        } else if (param_chunk->has_variable(variable)) {
            // here, we need to use the address in arg_chunk_pointer
            return frame->read_variable(Register::scratch, *arg_chunk_pointer) +
                   param_chunk->read_variable(reg, variable, Register::scratch);
        };

        assert(false && "Variable not found");
    };

    return lazy(evaluator);
}

Line Function::write_variable(const memory::Variable& variable, const Register& reg) const {
    auto evaluator = [reg, variable, this](Program& program) {
        if (frame->has_variable(variable)) {
            // frame is on top of the stack (otherwise we wouldn't be in this function)
            return frame->write_variable(variable, reg);
        } else if (param_chunk->has_variable(variable)) {
            // here, we need to use the address in arg_chunk_pointer
            return frame->read_variable(Register::scratch, *arg_chunk_pointer) +
                   param_chunk->write_variable(variable, reg, Register::scratch);
        };

        assert(false && "Variable not found");
    };

    return lazy(evaluator);
}

void Function::set_body(Line body) {
    this->body = std::move(body);
}
}  // namespace control