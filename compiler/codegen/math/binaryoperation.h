#ifndef _BINARY_OPERATION_
#define _BINARY_OPERATION_

#include "codegen/core/code.h"
#include "codegen/core/instruction.h"

// we could implement these as operator overloads, but concatenation is used quite often
// and wouldn't make sense with any operator other than +
namespace math {
Line add(Line operand1, Line operand2);
Line subtract(Line operand1, Line operand2);
Line multiply(Line operand1, Line operand2);
Line divide(Line operand1, Line operand2);  // signed division
Line modulo(Line operand1, Line operand2);
}  // namespace math

#endif