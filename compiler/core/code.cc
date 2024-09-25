#include "core/code.h"

#include <algorithm>

#include "core/program.h"

Code::Code(bool is_assembly)
    : _is_assembly{is_assembly} {}
Code::~Code() {}

bool Code::is_assembly() const {
    return _is_assembly;
}

// TODO see if we can prevent other declarations of this
std::ostream& operator<<(std::ostream& os, const Code& code) {
    return code.stream(os);
}

template <typename T>
std::vector<T> _combine_vectors(const std::vector<T>& v1, const std::vector<T>& v2) {
    std::vector<T> vec{};

    for (T t : v1) {
        vec.emplace_back(t);
    }

    for (T t : v2) {
        if (std::find(vec.begin(), vec.end(), t) == vec.end()) {
            vec.emplace_back(t);
        }
    }

    return vec;
}

class CombinedCode : public Code {
    // this keeps unique ownership of the codes, which requires them to be released from the original unique_ptrs
    Line line1;
    Line line2;

   public:
    CombinedCode(Line l1, Line l2)
        : Code{l1->is_assembly() && l2->is_assembly()},
          line1{std::move(l1)},
          line2{std::move(l2)} {}

    std::ostream& stream(std::ostream& os) const override {
        os << *line1 << std::endl;
        os << *line2;
        return os;
    }

    Line simplify(Program& program) override {
        Line simplified1{line1->simplify(program)};
        Line simplified2{line2->simplify(program)};

        return Line{
            new CombinedCode{
                simplified1 ? std::move(simplified1) : std::move(line1),
                simplified2 ? std::move(simplified2) : std::move(line2)}};
    }
};

class IncludeCode : public Code {
    Line base;
    const std::string include;

   public:
    IncludeCode(Line base, const std::string& include)
        : Code{base->is_assembly()},
          base{std::move(base)},
          include{include} {}

    std::ostream& stream(std::ostream& os) const override {
        return os << *base;
    };

    Line simplify(Program& program) override {
        program.add_include(include);

        Line simplified{base->simplify(program)};
        return simplified ? std::move(simplified) : std::move(base);
    };
};

Line operator+(Line l1, Line l2) {
    return Line{new CombinedCode{std::move(l1), std::move(l2)}};
}

Line with_include(Line line, const std::string& include) {
    return Line{new IncludeCode{std::move(line), include}};
}