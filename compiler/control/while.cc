#include "control/while.h"

#include "control/label.h"
#include "core/program.h"

While::While(Line condition, Line body) : Code{false}, condition{std::move(condition)}, body{std::move(body)} {}

Line While::simplify(Program& program) {
    Label* start = program.get_label();
    Label* end = program.get_label();

    return Line{start} +
           get_simplified(std::move(condition), program) +
           Line{new Instruction{std::format("cmp {}, #1", Register::arithmetic_result)}} +
           Line{new Instruction{"bne " + end->label()}} +
           get_simplified(std::move(body), program) +
           Line{new Instruction{"b " + start->label()}} +
           Line{end};
}

void While::allocate(Program& program) {
    condition->allocate(program);
    body->allocate(program);
}