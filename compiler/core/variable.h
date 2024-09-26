#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include <string>

// represents a variable
class Variable {
    std::string name;

   public:
    Variable(std::string name);

    bool operator==(const Variable& other) const;
    bool operator<(const Variable& other) const;
};
#endif