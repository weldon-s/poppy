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
    std::optional<control::Label> start_label;

   public:
    Function(std::vector<memory::Variable> arguments, Line body);
    Line call(std::vector<Line> arguments) const;
    Line declare(Program& program);
};

Line ret();
}  // namespace control

#endif