#include "core/program.h"

#include <algorithm>
#include <cassert>
#include <fstream>

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
    includes.push_back(include);
    return *this;
}

Program& Program::add_code(Line line) {
    code.push_back(std::move(line));

    // add the transformers needed by this code to the list of transformers
    // if they are not already there

    for (const Transformer* transformer : code.back()->needed_transformers()) {
        if (std::find(transformers.begin(), transformers.end(), transformer) == transformers.end()) {
            transformers.push_back(transformer);
        }
    }

    return *this;
}

Program& Program::compile(const std::string& name) {
    _apply_transformers();
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

void Program::_apply_transformers() {
    for (const Transformer* transformer : transformers) {
        for (int i = 0; i < code.size(); i++) {
            Line transformed = transformer->transform(std::move(code[i]), *this);
            code[i] = std::move(transformed);  // transfer ownership from transformed to code[i]
        }
    }

    // make sure all code is assembly
    for (const Line& c : code) {
        assert(c != nullptr && "Code cannot be null");
        assert(c->is_assembly() && "Code must be assembly");
    }
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
