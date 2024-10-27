#include "label.h"

#include "codegen/core/instruction.h"

namespace control {
Label::Label(const std::string& label) : _label{label} {}

Line Label::declare() const {
    return Line(new Instruction{_label + ":"});
}

Line Label::b() const {
    return Line(new Instruction{"b " + _label});
}

Line Label::bl() const {
    return Line(new Instruction{"bl " + _label});
}

Line Label::beq() const {
    return Line(new Instruction{"beq " + _label});
}

Line Label::bne() const {
    return Line(new Instruction{"bne " + _label});
}
}  // namespace control