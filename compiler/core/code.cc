#include "core/code.h"

#include <algorithm>

#include "core/transformer.h"

Code::Code(bool is_assembly, std::vector<const Transformer*> needed_transformers) : _is_assembly{is_assembly}, _needed_transformers{needed_transformers} {}
Code::~Code() {}

bool Code::is_assembly() const {
    return _is_assembly;
}

const std::vector<const Transformer*>& Code::needed_transformers() const {
    return _needed_transformers;
}

Line Code::apply_to_children(const Transformer& transformer, Program& program) {
    return Line{nullptr};
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
    // this keeps unique ownership of the codes, which requires them to be released from the original unique_ptrs
    Line line1;
    Line line2;

   public:
    CombinedCode(Line l1, Line l2)
        : Code{l1->is_assembly() && l2->is_assembly(), _combine_vectors(l1->needed_transformers(), l2->needed_transformers())},
          line1{std::move(l1)},
          line2{std::move(l2)} {}

    Line apply_to_children(const Transformer& transformer, Program& program) override {
        return transformer.transform(std::move(line1), program) + transformer.transform(std::move(line2), program);
    }

    std::ostream& stream(std::ostream& os) const override {
        os << *line1 << std::endl;
        os << *line2;
        return os;
    };
};

Line operator+(Line l1, Line l2) {
    return Line{new CombinedCode{std::move(l1), std::move(l2)}};
}