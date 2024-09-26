#include "core/variable.h"

Variable::Variable(std::string name) : name(name) {}

bool Variable::operator==(const Variable& other) const {
    return this == &other;
}

bool Variable::operator<(const Variable& other) const {
    return name < other.name;
}