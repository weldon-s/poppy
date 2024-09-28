#ifndef _IF_H_
#define _IF_H_

#include "core/code.h"

class If : public Code {
    Line condition;
    Line then_block;
    Line else_block;

    Line simplify(Program& program) override;

   public:
    If(Line condition, Line then_block, Line else_block = Line{nullptr});
    void allocate(Program& program) override;
};

#endif