#include "codegen/control/condition.h"

#include "codegen/core/program.h"
#include "codegen/core/register.h"
#include "codegen/memory/chunk.h"
#include "codegen/memory/variable.h"

using namespace control;

class Condition : public Code {
    Line operand1;
    Line operand2;
    Line operation;
    memory::Variable temp;
    Line declaration;

   public:
    Condition(Line operand1, Line operand2, Line operation)
        : Code{true}, operand1{std::move(operand1)}, operand2{std::move(operand2)}, operation{std::move(operation)}, temp{memory::Variable{}}, declaration{temp.declare()} {}

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
               get_simplified(std::move(operation), program);                                                    // perform the operation
    }
};

namespace control {
Line eq(Line l1, Line l2) {
    return Line{
        new Condition{std::move(l1), std::move(l2),
                      Line{
                          new Instruction{
                              Instruction{std::format("cmp {}, {}", Register::scratch, Register::arithmetic_result)} +
                              std::format("cset {}, eq", Register::arithmetic_result)}}}};
}

Line ne(Line l1, Line l2) {
    return Line{
        new Condition{std::move(l1), std::move(l2),
                      Line{
                          new Instruction{
                              Instruction{std::format("cmp {}, {}", Register::scratch, Register::arithmetic_result)} +
                              std::format("cset {}, ne", Register::arithmetic_result)}}}};
}

Line lt(Line l1, Line l2) {
    return Line{
        new Condition{std::move(l1), std::move(l2),
                      Line{
                          new Instruction{
                              Instruction{std::format("cmp {}, {}", Register::scratch, Register::arithmetic_result)} +
                              std::format("cset {}, lt", Register::arithmetic_result)}}}};
}

Line le(Line l1, Line l2) {
    return Line{
        new Condition{std::move(l1), std::move(l2),
                      Line{
                          new Instruction{
                              Instruction{std::format("cmp {}, {}", Register::scratch, Register::arithmetic_result)} +
                              std::format("cset {}, le", Register::arithmetic_result)}}}};
}

Line gt(Line l1, Line l2) {
    return Line{
        new Condition{std::move(l1), std::move(l2),
                      Line{
                          new Instruction{
                              Instruction{std::format("cmp {}, {}", Register::scratch, Register::arithmetic_result)} +
                              std::format("cset {}, gt", Register::arithmetic_result)}}}};
}

Line ge(Line l1, Line l2) {
    return Line{
        new Condition{std::move(l1), std::move(l2),
                      Line{
                          new Instruction{
                              Instruction{std::format("cmp {}, {}", Register::scratch, Register::arithmetic_result)} +
                              std::format("cset {}, ge", Register::arithmetic_result)}}}};
}

Line conj(Line l1, Line l2) {
    return Line{
        new Condition{std::move(l1), std::move(l2),
                      Line{new Instruction{std::format("and {}, {}, {}", Register::arithmetic_result, Register::scratch, Register::arithmetic_result)}}}};
}

Line disj(Line l1, Line l2) {
    return Line{
        new Condition{std::move(l1), std::move(l2),
                      Line{new Instruction{std::format("orr {}, {}, {}", Register::arithmetic_result, Register::scratch, Register::arithmetic_result)}}}};
}

Line neg(Line l) {
    // if the result is 0, we have 0 XOR 1 = 1
    // if the result is 1, we have 1 XOR 1 = 0
    return std::move(l) + Line{new Instruction{std::format("eor {}, {}, #1", Register::arithmetic_result, Register::arithmetic_result)}};
}
}  // namespace control