#ifndef _TRANSFORMER_H_
#define _TRANSFORMER_H_

class Code;
class Program;

class Transformer {
   public:
    // the rationale for the return ptr being const is that the code should not modified...instead, a new code should be created
    // this keeps things more predictable
    // program is NOT const because the transformer may need to modify the program (e.g. add includes or whatnot)
    // returns nullptr if no transformation is needed
    virtual const Code* transform(const Code* code, Program& program) = 0;
};

#endif