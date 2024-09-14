#ifndef _CODE_H_
#define _CODE_H_

// represents any piece of code (IR or assembly instruction) in the compiler
class Code {
    const bool _is_assembly;

   public:
    Code(bool is_assembly) : _is_assembly(is_assembly) {}
    bool is_assembly() const { return _is_assembly; }
    virtual ~Code() {}
};

#endif