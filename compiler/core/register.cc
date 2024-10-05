#include "core/register.h"

#include <stdexcept>

Register::Register(int reg) : reg{reg} {
    if ((reg < -1) || (reg > 30)) {
        throw std::invalid_argument("Invalid register number");
    }
}

Register::operator std::string() const {
    if (reg == stack_pointer.reg) {
        return "sp";
    } else if (reg == link_register.reg) {
        return "lr";
    } else if (reg == frame_pointer.reg) {
        return "fp";
    };

    return "x" + std::to_string(reg);
}

const Register& Register::scratch{Register{9}};
const Register& Register::arithmetic_result{Register{10}};
const Register& Register::arg_chunk_pointer{Register{11}};
const Register& Register::frame_pointer{Register{29}};
const Register& Register::link_register{Register{30}};

/* strictly speaking this is probably not the most "proper" OOP way to do this
   however, I don't think the added complexity would be worth it here since
   the logic of this class is SO simple */
const Register& Register::stack_pointer{Register{-1}};
