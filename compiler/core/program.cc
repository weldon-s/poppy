#include "core/program.h"

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

Program& Program::add_code(std::unique_ptr<const Code> line) {
    code.push_back(std::move(line));
    return *this;
}

const Program& Program::compile(const std::string& name) const {
    std::ofstream file(assembly_path + "/" + name + ".s");
    file << *this;
    file.close();
    return *this;
}

const Program& Program::compile() const {
    return compile("out");
}

const Program& Program::run() const {
    // run make
    system(("cd " + assembly_path + " && make").c_str());
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Program& program) {
    // first, we do includes
    for (std::string include : program.includes) {
        os << ".include \"" << include << ".s\"" << std::endl;
    }

    // then, we do the head
    os << Program::head << std::endl;

    // then, we do the code
    for (const std::unique_ptr<const Code>& line : program.code) {
        os << *line << std::endl;  // TODO handle this
    }

    // finally, we do the tail
    os << Program::tail << std::endl;

    return os;
}

void Program::_apply_transformers() {
    for (Transformer* transformer : transformers) {
        for (int i = 0; i < code.size(); i++) {
            std::unique_ptr<const Code> transformed = transformer->transform(*code[i], *this);

            // update the code if the transformer returned a new code and not nullptr
            if (transformed) {
                code[i].reset(transformed.release());  // transfer ownership from transformed to code[i]
            }
        }
    }

    // make sure all code is assembly
    for (const std::unique_ptr<const Code>& c : code) {
        assert(c != nullptr && "Code cannot be null");
        assert(c->is_assembly() && "Code must be assembly");
    }
}