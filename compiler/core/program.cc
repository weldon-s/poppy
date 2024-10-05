#include "core/program.h"

#include <algorithm>
#include <cassert>
#include <fstream>

#include "control/label.h"
#include "core/chunk.h"
#include "core/code.h"

const std::string Program::assembly_path = "../assembly";

const std::string Program::head =
    R""""(.text
.global _start
_start:
)"""";

const std::string Program::tail =
    R""""(mov x0, #0
mov w8, #93
svc #0)"""";

Program::Program() {}

Program& Program::add_include(const std::string& include) {
    if (std::find(includes.begin(), includes.end(), include) == includes.end()) {
        includes.push_back(include);
    }

    return *this;
}

Program& Program::add_code(Line line) {
    code.push_back(std::move(line));
    return *this;
}

Program& Program::add_variable(const Variable& v) {
    chunks.top()->add_variable(v);
    return *this;
}

control::Label Program::get_label() {
    return control::Label{"label" + std::to_string(label_count++)};
}

Program& Program::push_chunk(Chunk* chunk) {
    Chunk* previous{chunks.empty() ? nullptr : chunks.top()};
    chunks.push(chunk);
    chunk->set_previous(previous);
    return *this;
}

Program& Program::pop_chunk() {
    chunks.pop();
    return *this;
}

const Chunk& Program::top_chunk() {
    return *chunks.top();
}

Program& Program::compile(const std::string& name) {
    // allocate all variables
    for (const Line& c : code) {
        c->allocate(*this);
    }

    // simplify all code
    for (auto iter = code.begin(); iter != code.end(); iter++) {
        *iter = get_simplified(std::move(*iter), *this);
    }

    // make sure all code is assembly
    for (const Line& c : code) {
        assert(c != nullptr && "Code cannot be null");
        assert(c->is_assembly() && "Code must be assembly");
    }

    std::ofstream file(assembly_path + "/" + name + ".s");
    file << *this;
    file.close();
    return *this;
}

Program& Program::compile() {
    return compile("out");
}

Program& Program::run() {
    // run make
    system(("cd " + assembly_path + " && make").c_str());
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Program& program) {
    // first, we do includes
    for (std::string include : program.includes) {
        os << ".include \"" << include << ".s\"" << std::endl;
    }

    // then, we do the literals (data section)

    os << ".data" << std::endl;

    for (auto iter = program.literals.begin(); iter != program.literals.end(); iter++) {
        os << program._literal_label(iter) << ": .ascii \"" << *iter << "\"" << std::endl;
        os << program._literal_length_label(iter) << "= . - " << program._literal_label(iter) << std::endl;
    }

    // then, we do the head
    os << Program::head << std::endl;

    // then, we do the code
    for (const Line& line : program.code) {
        os << *line << std::endl;
    }

    // finally, we do the tail
    os << Program::tail << std::endl;

    return os;
}

std::pair<const std::string, const std::string> Program::add_literal(const std::string& literal) {
    auto iter{std::find(literals.begin(), literals.end(), literal)};

    if (iter == literals.end()) {
        literals.push_back(literal);
        iter = --literals.end();
    }

    return {_literal_label(iter), _literal_length_label(iter)};
}

const std::string Program::_literal_label(const std::vector<std::string>::const_iterator& iter) const {
    // these are based on the order in which the literals are added
    // so we CANT reorder them or we will break the program
    return "lit" + std::to_string(iter - literals.begin());
}

const std::string Program::_literal_length_label(const std::vector<std::string>::const_iterator& iter) const {
    // these are based on the order in which the literals are added
    // so we CANT reorder them or we will break the program
    return _literal_label(iter) + "len";
}
