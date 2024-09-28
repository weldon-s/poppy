#ifndef _CONDITION_H_
#define _CONDITION_H_

#include "core/code.h"
#include "core/variable.h"

class Condition : public Code {
    Line operand1;
    Line operand2;
    const std::string operation;
    Variable temp;
    Line declaration;

   public:
    Condition(Line operand1, Line operand2, const std::string operation);

    void allocate(Program& program) override;
    Line simplify(Program& program) override;
};

Line eq(Line l1, Line l2);
Line ne(Line l1, Line l2);
Line lt(Line l1, Line l2);
Line le(Line l1, Line l2);
Line gt(Line l1, Line l2);
Line ge(Line l1, Line l2);

#endif