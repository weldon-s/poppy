#include "core/code.h"

#include <algorithm>

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

std::vector<const Transformer*> _combine_vectors(const std::vector<const Transformer*>& v1, const std::vector<const Transformer*>& v2) {
    std::vector<const Transformer*> transformers{};

    for (const Transformer* t : v1) {
        transformers.emplace_back(t);
    }

    for (const Transformer* t : v2) {
        if (std::find(transformers.begin(), transformers.end(), t) == transformers.end()) {
            transformers.emplace_back(t);
        }
    }

    return transformers;
}

class CombinedCode : public Code {
    const Line& l1;
    const Line& l2;

   public:
    CombinedCode(const Line& l1, const Line& l2)
        : Code{l1->is_assembly() && l2->is_assembly(), _combine_vectors(l1->needed_transformers(), l2->needed_transformers())}, l1{l1}, l2{l2} {}

    std::ostream& stream(std::ostream& os) const override {
        os << l1 << std::endl;
        os << l2;
        return os;
    };
};

Line operator+(const Line& l1, const Line& l2) {
    return Line{new CombinedCode{l1, l2}};
}