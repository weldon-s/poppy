#include "core/code.h"

Code::Code(bool is_assembly) : _is_assembly{is_assembly} {}
Code::~Code() {}

bool Code::is_assembly() const {
    return _is_assembly;
}

// TODO see if we can prevent other declarations of this
std::ostream& operator<<(std::ostream& os, const Code& code) {
    return code.stream(os);
}
