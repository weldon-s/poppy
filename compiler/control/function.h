#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <vector>

#include "control/label.h"
#include "core/code.h"
#include "core/variable.h"

class Function {
    std::vector<Variable> arguments;
    Line body;
    Label* start_label;

   public:
    Function(std::vector<Variable> arguments, Line body);
    Line call(std::vector<Variable> arguments) const;
    Line declare(Program& program);
};

Line ret();

#endif