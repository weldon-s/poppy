#include "math/binaryoperation.h"

BinaryOperation::BinaryOperation(Line operand1, Line operand2, Line operation)
    : Code{false},
      operand1{std::move(operand1)},
      operand2{std::move(operand2)},
      operation{std::move(operation)} {}

Line BinaryOperation::simplify(Program& program) {
    return get_simplified(std::move(operand1), program) +
           push(Register::arithmetic_result) +
           get_simplified(std::move(operand2), program) +
           pop(Register::scratch) +
           std::move(operation);
}

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