#include "modules/popipo.h"

#include <format>
#include <iostream>

#include "core/instruction.h"
#include "core/program.h"
#include "core/transformer.h"

class PrintStr : public Code {
    const std::string str;

   public:
    PrintStr(const std::string& str);
    std::ostream& stream(std::ostream& os) const override;
    ~PrintStr();

    // this would be bad if these were declared + usable outside this file, but they are not
    friend class LiteralTransformer;
};

class LiteralTransformer : public Transformer {
   public:
    const std::unique_ptr<const Code> transform(const Code& code, Program& program) const override;
    static Transformer* instance();
};

PrintStr::PrintStr(const std::string& str) : Code(false, {LiteralTransformer::instance()}), str{str} {}
PrintStr::~PrintStr() {}

std::ostream& PrintStr::stream(std::ostream& os) const {
    return os << "print_str " << str << std::endl;
}

Transformer* LiteralTransformer::instance() {
    static LiteralTransformer instance;
    return &instance;
}
const std::unique_ptr<const Code> LiteralTransformer::transform(const Code& code, Program& program) const {
    const PrintStr* print_str_ptr = dynamic_cast<const PrintStr*>(&code);

    if (!print_str_ptr) {
        return nullptr;
    }

    std::pair<const std::string, const std::string> labels{program.add_literal(print_str_ptr->str)};

    Instruction* i = new Instruction(push(0) + push(1) + push(2) + push(8) +     // preserve values of x0, x1, x2, x8
                                     movi(0, 1) +                                // set up syscall arguments (file descriptor = 1, stdout)
                                     std::format("ldr x1, ={}", labels.first) +  // set up and call syscall
                                     std::format("ldr x2, ={}", labels.second) +
                                     "mov x8, #64" +
                                     "svc #0" +
                                     pop(8) + pop(2) + pop(1) + pop(0)  // restore values of x0, x1, x2, x8
    );

    return std::unique_ptr<const Code>(i);
}

std::unique_ptr<const Code> print_str(const std::string& str) {
    return std::unique_ptr<const Code>(new PrintStr{str});
}

std::unique_ptr<const Code> print_num(long long num) {
    Instruction* i = new Instruction(push(1) + movi(1, num) + "bl print_num" + pop(1));
    return std::unique_ptr<const Code>(i);
}
