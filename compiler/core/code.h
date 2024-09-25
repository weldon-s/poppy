#ifndef _CODE_H_
#define _CODE_H_

#include <iostream>
#include <memory>
#include <vector>

class Transformer;
class Program;

// represents any piece of code (IR or assembly instruction) in the compiler
class Code;
typedef std::unique_ptr<Code> Line;

class Code {
    const bool _is_assembly;
    const std::vector<const Transformer*> _needed_transformers;
    const std::vector<std::string> _needed_includes;
    virtual std::ostream& stream(std::ostream& os) const = 0;

   public:
    Code(bool is_assembly, std::vector<const Transformer*> needed_transformers = {}, std::vector<std::string> needed_includes = {});
    bool is_assembly() const;
    const std::vector<const Transformer*>& needed_transformers() const;

    // we could do this with Transformers, but this would be inefficient
    // since EVERY Code object in the program would trigger the adding of this include
    const std::vector<std::string>& needed_includes() const;

    // return Line{nullptr} if no children
    // this ensures we don't end up with multiple unique_ptrs to the same object
    // necessary because we implement this on Code (instead of on Line)
    virtual Line apply_to_children(const Transformer& transformer, Program& program);
    virtual ~Code();

    friend std::ostream& operator<<(std::ostream& os, const Code& code);
};

Line operator+(Line l1, Line l2);
Line with_include(Line line, const std::string& include);

#endif