#include "codegen/control/while.h"

#include "codegen/control/label.h"
#include "codegen/core/instruction.h"
#include "codegen/core/program.h"

class While : public Code {
    Line condition;
    Line body;

    Line simplify(Program& program) override {
        control::Label start = program.get_label();
        control::Label end = program.get_label();

        return start.declare() +
               get_simplified(std::move(condition), program) +
               Line{new Instruction{std::format("cmp {}, #1", Register::arithmetic_result)}} +
               end.bne() +
               get_simplified(std::move(body), program) +
               start.b() +
               end.declare();
    }

   public:
    While(Line condition, Line body)
        : Code{false}, condition{std::move(condition)}, body{std::move(body)} {}
    void allocate(Program& program) override {
        condition->allocate(program);
        body->allocate(program);
    };
};

namespace control {
Line whileloop(Line condition, Line body) {
    return Line{new While{std::move(condition), std::move(body)}};
}
}  // namespace control