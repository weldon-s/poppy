#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <optional>
#include <vector>

#include "control/label.h"
#include "core/code.h"
#include "memory/chunk.h"
#include "memory/variable.h"

namespace control {
class Function {
    std::vector<memory::Variable> arguments;
    Line body;
    control::Label start_label;
    std::unique_ptr<memory::Variable> caller_frame_pointer;
    std::unique_ptr<memory::Variable> saved_link;
    std::unique_ptr<memory::Variable> arg_chunk_pointer;
    std::unique_ptr<memory::Chunk> frame;
    std::unique_ptr<memory::Chunk> param_chunk;

   public:
    Function(const std::string name, std::vector<memory::Variable> arguments);
    Line call(std::vector<Line>& arguments) const;
    Line read_variable(const Register& reg, const memory::Variable& variable) const;
    Line write_variable(const memory::Variable& variable, const Register& reg) const;
    Line declare(Program& program);
    void set_body(Line body);
};
}  // namespace control

#endif