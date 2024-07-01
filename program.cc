#include "program.h"

#include <fstream>

const std::string Program::assembly_path = "assembly";

const std::string Program::head =
    R""""(.text
.global _start
_start:
mov x0, #1
)"""";

const std::string Program::tail =
    R""""(mov x0, #0
mov w8, #93
svc #0)"""";

Program::Program() {
    includes = std::vector<std::string>();
    code = std::vector<std::string>();
}

Program& Program::add_include(std::string include) {
    includes.push_back(include);
    return *this;
}

Program& Program::add_code(std::string line) {
    code.push_back(line);
    return *this;
}

void Program::compile(std::string name) const {
    std::ofstream file(assembly_path + "/" + name + ".s");
    file << *this;
    file.close();

    // run make
    system(("cd " + assembly_path + " && make").c_str());
}

void Program::compile() const {
    compile("out");
}

std::ostream& operator<<(std::ostream& os, const Program& program) {
    // first, we do includes
    for (std::string include : program.includes) {
        os << ".include \"" << include << ".s\"" << std::endl;
    }

    // then, we do the head
    os << Program::head << std::endl;

    // then, we do the code
    for (std::string line : program.code) {
        os << line << std::endl;
    }

    // finally, we do the tail
    os << Program::tail << std::endl;

    return os;
}