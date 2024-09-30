#ifndef _WHILE_H_
#define _WHILE_H_

#include "core/code.h"

class While : public Code {
    Line condition;
    Line body;

    Line simplify(Program& program) override;

   public:
    While(Line condition, Line body);
    void allocate(Program& program) override;
};

#endif