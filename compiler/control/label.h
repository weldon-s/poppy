#ifndef _LABEL_H_
#define _LABEL_H_

#include "core/code.h"
namespace control {
class Label {
    const std::string _label;

   public:
    explicit Label(const std::string& label);

    Line declare() const;
    Line b() const;
    Line bl() const;
    Line beq() const;
    Line bne() const;
};
}  // namespace control

#endif