#include "label.h"

std::ostream& Label::stream(std::ostream& os) const {
    return os << _label << ":";
}

Label::Label(const std::string& label) : Code(true), _label(label) {}

Line Label::simplify(Program& program) {
    return Line{nullptr};
}

const std::string& Label::label() const {
    return _label;
}
