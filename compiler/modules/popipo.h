#ifndef POPIPO_H
#define POPIPO_H

#include <memory>
#include <string>

#include "core/code.h"

// print_str should also have an overload taking in a register
// but that would involve arrays, which we haven't implemented yet
Line print_str(const std::string& str);
Line print_num(int reg);

#endif