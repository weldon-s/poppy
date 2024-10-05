#ifndef _IF_H_
#define _IF_H_

#include "core/code.h"

namespace control {
Line ifthen(Line condition, Line then_block);
Line ifthenelse(Line condition, Line then_block, Line else_block);
}  // namespace control

#endif