#ifndef _TRANSFORMER_H_
#define _TRANSFORMER_H_

#include <memory>

class Code;
class Program;

/*
this class could also be replaced by fn ptrs but this is better if we wanna expand
TODO add another field with other transformers this one must go before/after
or something idk
*/

class Transformer {
    virtual Line apply(Line line, Program &program) const = 0;

   public:
    Line transform(Line line, Program &program) const {
        Line children_applied{line->apply_to_children(*this, program)};

        if (children_applied.get() == nullptr) {
            return apply(std::move(line), program);
        }

        return apply(std::move(children_applied), program);
    }
    virtual ~Transformer() = default;
};

template <typename T>
class SingletonTransformer : public Transformer {
   protected:
    SingletonTransformer() = default;

   public:
    SingletonTransformer(const SingletonTransformer &) = delete;
    SingletonTransformer &operator=(const SingletonTransformer &) = delete;

    static Transformer *instance() {
        static T instance;
        return &instance;
    }
};

#endif