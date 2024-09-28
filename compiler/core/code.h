#ifndef _CODE_H_
#define _CODE_H_

#include <iostream>
#include <memory>
#include <vector>

class Program;

// represents any piece of code (IR or assembly instruction) in the compiler
class Code;
typedef std::unique_ptr<Code> Line;

class Code {
    const bool _is_assembly;

    // needs to be overridden if code is assembly
    virtual std::ostream& stream(std::ostream& os) const;

   public:
    Code(bool is_assembly);
    bool is_assembly() const;

    // process variable allocation (add variables/chunks)
    virtual void allocate(Program& program);

    /*
    return Line{nullptr} if unchanged
    this ensures we don't end up with multiple unique_ptrs to the same object
    necessary because we implement this on Code (instead of on Line)

    program is not const because we may need to modify it (e.g. add includes)
    */

    virtual Line simplify(Program& program) = 0;
    virtual ~Code();

    friend std::ostream& operator<<(std::ostream& os, const Code& code);
};

Line operator+(Line l1, Line l2);
Line with_include(Line line, const std::string& include);

#endif