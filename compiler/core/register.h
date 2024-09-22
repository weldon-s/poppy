#ifndef _REGISTER_H_
#define _REGISTER_H_

// represents a register
// this class helps disambiguate between registers and other integers/numbers
class Register {
   public:
    const int reg;
    explicit Register(int reg);  // prevent implicit conversion from int
};

const Register scratch{9};  // scratch register
#endif