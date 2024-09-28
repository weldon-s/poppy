#ifndef _BINARY_OPERATION_
#define _BINARY_OPERATION_

#include "core/code.h"
#include "core/instruction.h"

class BinaryOperation : public Code {
    Line operand1;
    Line operand2;
    Line operation;
    Line simplify(Program& program) override;

   public:
    BinaryOperation(Line operand1, Line operand2, Line operation);
};

// we could implement these as overloads, but concatenation is used quite often
// and wouldn't make sense with any operator other than +
Line add(Line operand1, Line operand2);
Line subtract(Line operand1, Line operand2);
Line multiply(Line operand1, Line operand2);
Line divide(Line operand1, Line operand2);  // signed division
Line modulo(Line operand1, Line operand2);

#endif