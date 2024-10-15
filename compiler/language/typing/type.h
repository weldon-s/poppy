#ifndef _TYPE_H_
#define _TYPE_H_

#include <string>
#include <vector>
namespace lang {
class Type {
    const std::string _name;

   private:
    Type(std::string name);

   public:
    std::string name() const;
    constexpr bool operator==(const Type &other);
    constexpr bool is_function() const;

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