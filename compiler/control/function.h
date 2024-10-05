#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include <vector>

#include "control/label.h"
#include "core/code.h"
#include "memory/variable.h"

class Function {
    std::vector<memory::Variable> arguments;
    Line body;
    control::Label* start_label;

   public:
    Function(std::vector<memory::Variable> arguments, Line body);
    Line call(std::vector<memory::Variable> arguments) const;
    Line declare(Program& program);
};

Line ret();

#endif