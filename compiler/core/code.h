#ifndef _CODE_H_
#define _CODE_H_

#include <iostream>
#include <vector>

class Transformer;

// represents any piece of code (IR or assembly instruction) in the compiler
// all code objects should be immutable
class Code {
    const bool _is_assembly;
    std::vector<const Transformer*> _needed_transformers;
    virtual std::ostream& stream(std::ostream& os) const = 0;

   public:
    Code(bool is_assembly, std::vector<const Transformer*> needed_transformers = {});
    bool is_assembly() const;
    const std::vector<const Transformer*>& needed_transformers() const;
    // TODO consider adding operator+
    //  would operate with pointers -> clarity might be lost

    virtual ~Code();

    friend std::ostream& operator<<(std::ostream& os, const Code& code);
};

#endif