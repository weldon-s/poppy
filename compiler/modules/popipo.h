#ifndef POPIPO_H
#define POPIPO_H

#include <memory>
#include <string>

#include "core/code.h"

std::unique_ptr<const Code> print_str(const std::string& str);
std::unique_ptr<const Code> print_num(long long num);

#endif