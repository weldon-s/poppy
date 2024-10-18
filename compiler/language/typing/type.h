#ifndef _TYPE_H_
#define _TYPE_H_

#include <string>
#include <vector>
namespace lang {
class Type {
    std::string _name;
    Type(std::string name);

   public:
    std::string name() const;
    constexpr bool operator==(const Type &other) const {
        return _name == other._name;
    }

    constexpr bool is_function() const {
        return _name[0] == '(';
    }

    // undefined behavior if not a function type
    Type return_type() const;
    std::vector<Type> arg_types() const;

    const static Type INT;
    const static Type CHAR;
    const static Type STRING;
    const static Type VOID;
    const static Type BOOL;
    const static Type NONE;
    static Type function(Type return_type, std::vector<Type> args);
};
}  // namespace lang

#endif