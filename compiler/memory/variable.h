#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include <string>

#include "core/code.h"

namespace memory {
// represents a variable
class Variable {
    static int counter;

   public:
    Variable();
    explicit Variable(std::string name);

    bool operator==(const Variable& other) const;
    bool operator<(const Variable& other) const;

    Line declare() const;
    const std::string name;
};
}  // namespace memory
#endif