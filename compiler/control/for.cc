#include "control/for.h"

#include "control/label.h"
#include "control/while.h"
#include "core/program.h"

For::For(Line initialization, Line condition, Line advancement, Line body)
    : Code{false},
      initialization{std::move(initialization)},
      condition{std::move(condition)},
      advancement{std::move(advancement)},
      body{std::move(body)} {}

void For::allocate(Program& program) {
    initialization->allocate(program);
    condition->allocate(program);
    advancement->allocate(program);
    body->allocate(program);
}

Line For::simplify(Program& program) {
    return get_simplified(std::move(initialization), program) +
           get_simplified(Line{
                              new While{
                                  std::move(condition),
                                  std::move(body) + std::move(advancement)}},
                          program);
}