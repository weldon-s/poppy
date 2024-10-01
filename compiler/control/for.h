#ifndef _FOR_H_
#define _FOR_H_

#include "core/code.h"

class For : public Code {
    Line initialization;
    Line condition;
    Line advancement;
    Line body;

    Line simplify(Program& program) override;

   public:
    For(Line initialization, Line condition, Line advancement, Line body);
    void allocate(Program& program) override;
};

#endif