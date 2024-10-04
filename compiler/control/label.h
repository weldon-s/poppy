#ifndef _LABEL_H_
#define _LABEL_H_

#include "core/code.h"

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

#endif