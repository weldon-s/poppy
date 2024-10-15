#include "type.h"

namespace lang {
const Type Type::INT = Type("int");
const Type Type::CHAR = Type("char");
const Type Type::STRING = Type("string");
const Type Type::VOID = Type("void");
const Type Type::BOOL = Type("bool");
const Type Type::NONE = Type("none");

Type::Type(std::string name) : _name(name) {}

std::string Type::name() const {
    return _name;
}

constexpr bool Type::operator==(const Type &other) {
    return _name == other._name;
}

constexpr bool Type::is_function() const {
    return _name[0] == '(';
}

Type Type::function(Type return_type, std::vector<Type> args) {
    std::string name = "(";
    for (size_t i = 0; i < args.size(); i++) {
        name += args[i].name();
        if (i < args.size() - 1) {
            name += ", ";
        }
    }
    name += ") -> ";
    name += return_type.name();
    return Type(name);
}
}  // namespace lang