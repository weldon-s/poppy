#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "symbol.h"

struct rule {
        enum symbol lhs;
        enum symbol *rhs;
        int rhs_len;
};

struct grammar {
        enum symbol start;
        struct rule *rules;
        int rules_len;
        bool nullable[SYMBOL_COUNT];
};

#endif