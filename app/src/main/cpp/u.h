//
// Created by skye on 1/28/26.
//

#ifndef CONIC_U_H
#define CONIC_U_H

#include <cctype>
#include <cstdint>
#include <cstddef>

#define inl [[clang::always_inline]] inline
#define Z(x) (sizeof(x))
#define fatal(f...) {fprintf(stderr, "[FATAL]: " f);putc('\n', stderr);exit(-1);}

using S = size_t;

namespace U {
    extern const char *VERSION;
}

#endif
