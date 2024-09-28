#include "control/if.h"

#include "control/label.h"
#include "core/program.h"

If::If(Line condition, Line then_block, Line else_block)
    : Code{false},
      condition{std::move(condition)},
      then_block{std::move(then_block)},
      else_block{std::move(else_block)} {}

Line If::simplify(Program& program) {
    Label* after_then = program.get_label();
    Label* after_else = program.get_label();

    Line beginning =
        get_simplified(std::move(condition), program) +
        Line(new Instruction{std::format("cmp {}, #1", Register::arithmetic_result)}) +
        Line(new Instruction{"bne " + after_then->label()}) +
        get_simplified(std::move(then_block), program);

    if (else_block) {
        return std::move(beginning) +
               Line(new Instruction{"b " + after_else->label()}) +
               Line(after_then) +
               get_simplified(std::move(else_block), program) +
               Line(after_else);
    }

    return std::move(beginning) + Line(after_then);
}

void If::allocate(Program& program) {
    condition->allocate(program);
    then_block->allocate(program);

    if (else_block) {
        else_block->allocate(program);
    }
}