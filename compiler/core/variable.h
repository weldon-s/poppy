#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include <string>

#include "core/code.h"

// represents a variable
class Variable {
    static int counter;
    const std::string name;

   public:
    Variable();
    explicit Variable(std::string name);

    bool operator==(const Variable& other) const;
    bool operator<(const Variable& other) const;

    Line declare() const;
};
#endif