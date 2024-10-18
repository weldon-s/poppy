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

Type Type::return_type() const {
    size_t start = _name.find("-> ") + 3;
    return Type(_name.substr(start));
}

std::vector<Type> Type::arg_types() const {
    size_t start = _name.find("(") + 1;
    size_t end = _name.find(")");
    std::string args = _name.substr(start, end - start);
    std::vector<Type> result;
    size_t pos = 0;
    while (pos < args.size()) {
        size_t comma = args.find(", ", pos);
        if (comma == std::string::npos) {
            comma = args.size();
        }
        result.push_back(Type(args.substr(pos, comma - pos)));
        pos = comma + 2;
    }
    return result;
}
}  // namespace lang