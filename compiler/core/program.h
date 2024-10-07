#ifndef PROGRAM_H
#define PROGRAM_H
// represents a Poppy program
#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <vector>

#include "core/code.h"
namespace memory {
class Chunk;
class Variable;
}  // namespace memory

namespace control {
class Label;
}  // namespace control

class Program {
    // assembly path
    static const std::string assembly_path;

    // important bits that define _start, end gracefully
    static const std::string head;
    static const std::string tail;

    // dependencies for the program (assembly files without the .s extension)
    std::vector<std::string> includes;

    // the code of the program
    std::vector<Line> code;

    // the string literals of the program. these are stored in the data section
    std::vector<std::string> literals;

    // the chunks currently in use by the program
    // note: this is only used to allocate variables, hence no ownership is needed
    std::stack<memory::Chunk*> chunks;

    int label_count{0};

    const std::string _literal_label(const std::vector<std::string>::const_iterator& iter) const;
    const std::string _literal_length_label(const std::vector<std::string>::const_iterator& iter) const;

   public:
    Program();
    Program& add_include(const std::string& include);
    Program& add_code(const Line line);
    Program& add_variable(const memory::Variable& v);
    Program& push_chunk(memory::Chunk* chunk);
    Program& pop_chunk();
    memory::Chunk& top_chunk();
    control::Label get_label();

    Program& compile(const std::string& name);  // compiles the program into an executable
    Program& compile();
    Program& run();  // runs the program

    static Line start();  // marks the start of the program

    // adds a literal to the program and returns its label and its length's label
    std::pair<const std::string, const std::string> add_literal(const std::string& literal);
    friend std::ostream& operator<<(std::ostream& os, const Program& program);
};

#endif