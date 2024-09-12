#ifndef POPIPO_H
#define POPIPO_H

#include <string>

#include "core/instruction.h"

Instruction print_str(const std::string& name, const std::string& str);
Instruction print_num(long long num);

#endif