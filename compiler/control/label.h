#ifndef _LABEL_H_
#define _LABEL_H_

#include "core/code.h"

class Label : public Code {
    const std::string _label;
    std::ostream& stream(std::ostream& os) const override;
    Line simplify(Program& program) override;

   public:
    explicit Label(const std::string& label);
    const std::string& label() const;
};

#endif