#include "codegen/core/code.h"

#include <algorithm>
#include <cassert>

#include "codegen/core/program.h"

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

void Code::allocate(Program& program) {}

std::ostream& Code::stream(std::ostream& os) const {
    throw std::runtime_error("stream not implemented");
}

class CombinedCode : public Code {
    // this keeps unique ownership of the codes, which requires them to be released from the original unique_ptrs
    Line line1;
    Line line2;

   public:
    CombinedCode(Line l1, Line l2)
        : Code{l1->is_assembly() && l2->is_assembly()},
          line1{std::move(l1)},
          line2{std::move(l2)} {
        assert(line1 != nullptr && "line1 cannot be null");
        assert(line2 != nullptr && "line2 cannot be null");
    }

    std::ostream& stream(std::ostream& os) const override {
        os << *line1 << std::endl;
        os << *line2;
        return os;
    }

    void allocate(Program& program) override {
        line1->allocate(program);
        line2->allocate(program);
    }

    Line simplify(Program& program) override {
        return Line{
            new CombinedCode{
                get_simplified(std::move(line1), program),
                get_simplified(std::move(line2), program)}};
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

    void allocate(Program& program) override {
        base->allocate(program);
    }

    Line simplify(Program& program) override {
        program.add_include(include);

        return get_simplified(std::move(base), program);
    };
};

class LazilyEvaluatedCode : public Code {
    const std::function<Line()> evaluator;

   public:
    LazilyEvaluatedCode(const std::function<Line()> evaluator)
        : Code{false}, evaluator{evaluator} {}

    Line simplify(Program& program) override {
        return get_simplified(evaluator(), program);
    }
};

class LazilyEvaluatedProgramCode : public Code {
    const std::function<Line(Program&)> evaluator;

   public:
    LazilyEvaluatedProgramCode(const std::function<Line(Program&)> evaluator)
        : Code{false}, evaluator{evaluator} {}

    Line simplify(Program& program) override {
        return get_simplified(evaluator(program), program);
    }
};

Line operator+(Line l1, Line l2) {
    return Line{new CombinedCode{std::move(l1), std::move(l2)}};
}

Line with_include(Line line, const std::string& include) {
    return Line{new IncludeCode{std::move(line), include}};
}

Line lazy(std::function<Line(Program&)> f) {
    return Line{new LazilyEvaluatedProgramCode{f}};
}

Line lazy(std::function<Line()> f) {
    return Line{new LazilyEvaluatedCode{f}};
}

Line get_simplified(Line line, Program& program) {
    Line simplified{line->simplify(program)};

    if (simplified) {
        return simplified;
    }

    return line;
}