#ifndef _REGISTER_H_
#define _REGISTER_H_

#include <format>
#include <string>

// represents a register
// this class helps disambiguate between registers and other integers/numbers
class Register {
   public:
    Register(int reg);  // once we have more named registers, we could make this private

    const int reg;
    // note: this only handles 64-bit access...maybe we also want 32 bits?
    // TODO think about this
    operator std::string() const;

    static const Register& scratch;
    static const Register& frame_pointer;
    static const Register& link_register;
    static const Register& stack_pointer;
};

template <>
struct std::formatter<Register> {
    constexpr auto parse(std::format_parse_context& ctx) {
        return ctx.begin();
    }

    auto format(const Register& reg, std::format_context& ctx) const {
        return std::format_to(ctx.out(), "{}", static_cast<std::string>(reg));
    }
};
#endif