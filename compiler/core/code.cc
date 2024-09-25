#include "core/code.h"

#include <algorithm>

#include "core/transformer.h"

Code::Code(bool is_assembly, std::vector<const Transformer*> needed_transformers, std::vector<std::string> needed_includes)
    : _is_assembly{is_assembly}, _needed_transformers{needed_transformers}, _needed_includes{needed_includes} {}
Code::~Code() {}

bool Code::is_assembly() const {
    return _is_assembly;
}

const std::vector<const Transformer*>& Code::needed_transformers() const {
    return _needed_transformers;
}

const std::vector<std::string>& Code::needed_includes() const {
    return _needed_includes;
}

Line Code::apply_to_children(const Transformer& transformer, Program& program) {
    return Line{nullptr};
}

// TODO see if we can prevent other declarations of this
std::ostream& operator<<(std::ostream& os, const Code& code) {
    return code.stream(os);
}

template <typename T>
std::vector<T> _combine_vectors(const std::vector<T>& v1, const std::vector<T>& v2) {
    std::vector<T> transformers{};

    for (T t : v1) {
        transformers.emplace_back(t);
    }

    for (T t : v2) {
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
        : Code{l1->is_assembly() && l2->is_assembly(), _combine_vectors(l1->needed_transformers(), l2->needed_transformers()),
               _combine_vectors(l1->needed_includes(), l2->needed_includes())},
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

class IncludeCode : public Code {
    Line base;

    template <typename T>
    static std::vector<T> _append(const std::vector<T>& v, const T& s) {
        std::vector<T> includes{v};

        if (std::find(includes.begin(), includes.end(), s) == includes.end()) {
            includes.emplace_back(s);
        }

        return includes;
    }

   public:
    IncludeCode(Line base, const std::string& include)
        : Code{base->is_assembly(), base->needed_transformers(), _append(base->needed_includes(), include)},
          base{std::move(base)} {}

    Line apply_to_children(const Transformer& transformer, Program& program) override {
        return transformer.transform(std::move(base), program);
    }

    std::ostream& stream(std::ostream& os) const override {
        return os << *base;
    };
};

Line with_include(Line line, const std::string& include) {
    return Line{new IncludeCode{std::move(line), include}};
}