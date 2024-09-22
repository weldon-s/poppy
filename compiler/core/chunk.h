#ifndef CHUNK
#define CHUNK

#include <map>
#include <string>
#include <vector>

#include "instruction.h"

// represents a variable
class Variable {
    std::string name;

   public:
    Variable(std::string name);

    bool operator==(const Variable& other) const;
    bool operator<(const Variable& other) const;
};

// represents a chunk allocated on the stack
class Chunk {
    std::map<Variable, int> offsets;
    int size;

   public:
    Chunk(std::vector<Variable> variables);

    Line push_chunk() const;
    Line pop_chunk() const;
    Line read_variable(int reg, const Variable& variable) const;
    Line write_variable(const Variable& variable, int reg) const;
};

#endif