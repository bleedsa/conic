#ifndef CONIC_R_H
#define CONIC_R_H

#include "str.h"

template<typename X>
struct R {
    bool I;
    union {
        X x;
        Str::A err;
    };

    inline explicit R(X x)            : I{true}, x{x} {}
    inline explicit R(const char *x)  : I{false}, err{Str::A(x)} {}
    inline explicit R(std::string &x) : I{false}, err{Str::A(x)} {}

    inline ~R() {
        if (I) x.~X();
        else err.~A();
    }

    inl auto cpy(R<X> &x, const R<X> &y) -> void {
        x.I=y.I;
        if (x.I) x.x=y.x;
        else x.err=y.err;
    }

    inl R(const R<X> &x) {
        cpy(*this, x);
    }

    inl auto operator=(const R<X> &x) -> R<X>& {
        cpy(*this, x);
        return *this;
    }
};

template<typename T>
inl auto operator!(const R<T> &x) -> bool {
    return !x.I;
}

template<typename T>
inl auto operator*(const R<T> &x) -> const T& {
    return x.x;
}

#endif
