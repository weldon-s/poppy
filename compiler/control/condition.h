#ifndef _CONDITION_H_
#define _CONDITION_H_

#include "core/code.h"
namespace control {
Line eq(Line l1, Line l2);
Line ne(Line l1, Line l2);
Line lt(Line l1, Line l2);
Line le(Line l1, Line l2);
Line gt(Line l1, Line l2);
Line ge(Line l1, Line l2);
}  // namespace control

#endif