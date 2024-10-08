#include "control/condition.h"

#include "core/program.h"
#include "core/register.h"
#include "memory/chunk.h"
#include "memory/variable.h"

using namespace control;

class Condition : public Code {
    Line operand1;
    Line operand2;
    const std::string operation;
    memory::Variable temp;
    Line declaration;

   public:
    Condition(Line operand1, Line operand2, const std::string operation)
        : Code{true}, operand1{std::move(operand1)}, operand2{std::move(operand2)}, operation{operation}, temp{memory::Variable{}}, declaration{temp.declare()} {}

    void allocate(Program& program) override {
        declaration->allocate(program);
        operand1->allocate(program);
        operand2->allocate(program);
    }

    Line simplify(Program& program) override {
        return get_simplified(std::move(declaration), program) +                                                 // declare the temporary variable
               get_simplified(std::move(operand1), program) +                                                    // evaluate the first operand
               get_simplified(program.top_chunk().write_variable(temp, Register::arithmetic_result), program) +  // store the result in the temporary variable
               get_simplified(std::move(operand2), program) +                                                    // evaluate the second operand
               get_simplified(program.top_chunk().read_variable(Register::scratch, temp), program) +             // load the result back into the scratch register
               get_simplified(Line{new Instruction{
                                  Instruction{std::format("cmp {}, {}", Register::scratch, Register::arithmetic_result)} +
                                  std::format("cset {}, {}", Register::arithmetic_result, operation)}},
                              program);  // perform the operation
    }
};

namespace control {
Line eq(Line l1, Line l2) {
    return Line{new Condition{std::move(l1), std::move(l2), "eq"}};
}

Line ne(Line l1, Line l2) {
    return Line{new Condition{std::move(l1), std::move(l2), "ne"}};
}

Line lt(Line l1, Line l2) {
    return Line{new Condition{std::move(l1), std::move(l2), "lt"}};
}

Line le(Line l1, Line l2) {
    return Line{new Condition{std::move(l1), std::move(l2), "le"}};
}

Line gt(Line l1, Line l2) {
    return Line{new Condition{std::move(l1), std::move(l2), "gt"}};
}

Line ge(Line l1, Line l2) {
    return Line{new Condition{std::move(l1), std::move(l2), "ge"}};
}
}  // namespace control