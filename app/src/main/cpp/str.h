#ifndef CONIC_STR_H
#define CONIC_STR_H

#include <cstring>
#include <string>

#include "u.h"

namespace Str {
    struct A {
        S len;
        char *ptr;

        explicit A(const char *x);
        explicit A(const std::string &x);
        ~A();

        A(const A &x);
        const A &operator=(const A &x);

        [[nodiscard]]
        inl auto Zof() const -> S {
            return Z(char)*(1+len);
        }

        inl auto cpy(const char *x) -> void {
            memmove(ptr, x, Zof());
        }

        [[nodiscard]]
        inl auto c_str() const -> const char* {
            auto str = new char[len];
            memmove(str, ptr, Zof());
            return str;
        }
    };
}

#endif
