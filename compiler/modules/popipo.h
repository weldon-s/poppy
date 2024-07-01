#ifndef POPIPO_H
#define POPIPO_H

#include <string>

#include "../instruction.h"

Instruction print_str(std::string name, std::string str);
Instruction print_num(long long num);

#endif