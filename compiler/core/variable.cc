#include "core/variable.h"

#include "core/program.h"

int Variable::counter = 0;

Variable::Variable() : name{"var" + std::to_string(counter++)} {}

Variable::Variable(std::string name) : name{name} {}

bool Variable::operator==(const Variable& other) const {
    return this == &other;
}

bool Variable::operator<(const Variable& other) const {
    return name < other.name;
}

class Declaration : public Code {
    const Variable& variable;

   public:
    Declaration(const Variable& variable) : Code{true}, variable{variable} {}

    virtual std::ostream& stream(std::ostream& os) const override {
        // this does not map to any assembly instruction
        return os;
    }

    virtual Line simplify(Program& program) override {
        return Line{nullptr};
    }

    virtual void allocate(Program& program) override {
        program.add_variable(variable);
    }
};

Line Variable::declare() const {
    return Line{new Declaration{*this}};
}