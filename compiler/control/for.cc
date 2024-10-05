#include "control/for.h"

#include "control/label.h"
#include "control/while.h"
#include "core/program.h"

class For : public Code {
    Line initialization;
    Line condition;
    Line advancement;
    Line body;

    Line simplify(Program& program) override {
        return get_simplified(std::move(initialization), program) +
               get_simplified(
                   control::whileloop(
                       std::move(condition),
                       std::move(body) + std::move(advancement)),
                   program);
    }

   public:
    For(Line initialization, Line condition, Line advancement, Line body)
        : Code{false},
          initialization{std::move(initialization)},
          condition{std::move(condition)},
          advancement{std::move(advancement)},
          body{std::move(body)} {}

    void allocate(Program& program) override {
        initialization->allocate(program);
        condition->allocate(program);
        advancement->allocate(program);
        body->allocate(program);
    };
};

Line control::forloop(Line initialization, Line condition, Line advancement, Line body) {
    return Line{new For{std::move(initialization), std::move(condition), std::move(advancement), std::move(body)}};
}