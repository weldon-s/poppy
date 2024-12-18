#include "codegen/math/binaryoperation.h"

class BinaryOperation : public Code {
    Line operand1;
    Line operand2;
    Line operation;
    void allocate(Program& program) override {
        operand1->allocate(program);
        operand2->allocate(program);
        operation->allocate(program);
    };

    Line simplify(Program& program) override {
        return get_simplified(std::move(operand1), program) +
               assem::push(Register::arithmetic_result) +
               get_simplified(std::move(operand2), program) +
               assem::pop(Register::scratch) +
               std::move(operation);
    };

   public:
    BinaryOperation(Line operand1, Line operand2, Line operation) : Code{false},
                                                                    operand1{std::move(operand1)},
                                                                    operand2{std::move(operand2)},
                                                                    operation{std::move(operation)} {}
};

namespace math {
Line add(Line operand1, Line operand2) {
    return Line{
        new BinaryOperation{
            std::move(operand1), std::move(operand2), Line{new Instruction{"add x10, x9, x10"}}}};
}

Line subtract(Line operand1, Line operand2) {
    return Line{
        new BinaryOperation{
            std::move(operand1), std::move(operand2), Line{new Instruction{"sub x10, x9, x10"}}}};
}

Line multiply(Line operand1, Line operand2) {
    return Line{
        new BinaryOperation{
            std::move(operand1), std::move(operand2), Line{new Instruction{"mul x10, x9, x10"}}}};
}

Line divide(Line operand1, Line operand2) {
    return Line{
        new BinaryOperation{
            std::move(operand1), std::move(operand2), Line{new Instruction{"sdiv x10, x9, x10"}}}};
}

// initially, x9 has original number (n), x10 has divisor (d)
// x11 <- n / d
// x10 <- n - d * (n / d)

Line modulo(Line operand1, Line operand2) {
    return Line{
        new BinaryOperation{
            std::move(operand1), std::move(operand2),
            Line{new Instruction{"sdiv x11, x9, x10"}} +
                Line{
                    new Instruction{"msub x10, x10, x11, x9"}}}};
}
}  // namespace math