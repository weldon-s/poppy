#include "codegen/control/if.h"

#include "codegen/control/label.h"
#include "codegen/core/instruction.h"
#include "codegen/core/program.h"

class If : public Code {
    Line condition;
    Line then_block;
    Line else_block;

    Line simplify(Program& program) override {
        control::Label after_then = program.get_label();
        control::Label after_else = program.get_label();

        Line beginning =
            get_simplified(std::move(condition), program) +
            Line(new Instruction{std::format("cmp {}, #1", Register::arithmetic_result)}) +
            after_then.bne() +
            get_simplified(std::move(then_block), program);

        if (else_block) {
            return std::move(beginning) +
                   after_else.b() +
                   after_then.declare() +
                   get_simplified(std::move(else_block), program) +
                   after_else.declare();
        }

        return std::move(beginning) + after_then.declare();
    }

   public:
    If(Line condition, Line then_block, Line else_block = Line{nullptr})
        : Code{false},
          condition{std::move(condition)},
          then_block{std::move(then_block)},
          else_block{std::move(else_block)} {}

    void allocate(Program& program) override {
        condition->allocate(program);
        then_block->allocate(program);

        if (else_block) {
            else_block->allocate(program);
        }
    }
};

namespace control {
Line ifthen(Line condition, Line then_block) {
    return Line{new If{std::move(condition), std::move(then_block)}};
}

Line ifthenelse(Line condition, Line then_block, Line else_block) {
    return Line{new If{std::move(condition), std::move(then_block), std::move(else_block)}};
}
}  // namespace control