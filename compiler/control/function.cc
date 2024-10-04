#include "control/function.h"

#include "core/program.h"

Function::Function(std::vector<Variable> arguments, Line body) : arguments{arguments}, body{std::move(body)}, start_label{nullptr} {}

Line Function::declare(Program& program) {
    start_label = &program.get_label();
    return start_label->declare() + get_simplified(std::move(body), program);
}

Line ret() {
    return Line(new Instruction{"ret"});
}