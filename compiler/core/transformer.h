#ifndef _TRANSFORMER_H_
#define _TRANSFORMER_H_

#include <memory>

class Code;
class Program;

/*
this class could also be replaced by fn ptrs but this is better if we wanna expand
TODO make singleton (CRTP?)
TODO add another field with other transformers this one must go before/after
or something idk
*/

class Transformer {
   public:
    /*
    the rationale for the return ptr being const is that the code should not modified...instead, a new code should be created
    this keeps things more predictable
    program is NOT const because the transformer may need to modify the program (e.g. add includes or whatnot)
    returns nullptr if no transformation is needed
    */
    virtual const std::unique_ptr<const Code> transform(const Code& code, Program& program) = 0;
};

#endif