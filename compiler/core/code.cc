#include "core/code.h"

Code::Code(bool is_assembly, std::vector<const Transformer*> needed_transformers) : _is_assembly{is_assembly}, _needed_transformers{needed_transformers} {}
Code::~Code() {}

bool Code::is_assembly() const {
    return _is_assembly;
}

const std::vector<const Transformer*>& Code::needed_transformers() const {
    return _needed_transformers;
}

// TODO see if we can prevent other declarations of this
std::ostream& operator<<(std::ostream& os, const Code& code) {
    return code.stream(os);
}
