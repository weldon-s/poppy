#ifndef _CODE_H_
#define _CODE_H_

#include <iostream>
#include <memory>
#include <vector>

class Transformer;

// represents any piece of code (IR or assembly instruction) in the compiler
// all code objects should be immutable

class Code {
    const bool _is_assembly;
    const std::vector<const Transformer*> _needed_transformers;
    virtual std::ostream& stream(std::ostream& os) const = 0;

   public:
    Code(bool is_assembly, std::vector<const Transformer*> needed_transformers = {});
    bool is_assembly() const;
    const std::vector<const Transformer*>& needed_transformers() const;
    virtual ~Code();

    friend std::ostream& operator<<(std::ostream& os, const Code& code);
};

typedef std::unique_ptr<const Code> Line;

Line operator+(Line l1, Line l2);

#endif