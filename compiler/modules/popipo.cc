#include "modules/popipo.h"

#include <format>
#include <map>

#include "core/instruction.h"
#include "core/program.h"

const std::map<char, char> escape_map{
    {'\'', '\''}, {'\"', '\"'}, {'\?', '?'}, {'\\', '\\'}, {'\a', 'a'}, {'\b', 'b'}, {'\f', 'f'}, {'\n', 'n'}, {'\r', 'r'}, {'\t', 't'}, {'\v', 'v'}};

const std::string escape(const std::string& str) {
    std::string escaped;
    for (size_t i = 0; i < str.size(); i++) {
        if (escape_map.contains(str[i])) {
            escaped += '\\';
            escaped += escape_map.at(str[i]);
        } else {
            escaped += str[i];
        }
    }

    return escaped;
}

class PrintStr : public Code {
    const std::string str;

   public:
    PrintStr(const std::string& str) : Code{false}, str{str} {}
    Line simplify(Program& program) override {
        std::pair<const std::string, const std::string> labels{program.add_literal(escape(str))};

        return push_pair(Register(0), Register(1)) + push_pair(Register(2), Register(8)) +  // preserve values of x0, x1, x2, x8
               movi(Register(0), 1) +
               Line{
                   new Instruction{
                       Instruction{std::format("ldr x1, ={}", labels.first)} +  // set up and call syscall
                       std::format("ldr x2, ={}", labels.second) +
                       "mov x8, #64" +
                       "svc #0"}} +
               pop_pair(Register(8), Register(2)) + pop_pair(Register(1), Register(0));  // restore values of x0, x1, x2, x8
    }
};

Line print_str(const std::string& str) {
    return Line(new PrintStr{str});
}

Line print_num(const Register& reg) {
    return with_include(push(Register(1)) + mov(Register(1), reg) + Line{new Instruction{"bl print_num"}} + pop(Register(1)), "print_num");
}