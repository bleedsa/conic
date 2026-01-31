#include "str.h"

Str::A::A(const char *x) : len{strlen(x)}, ptr{new char[Zof()]} {
    cpy(x);
}

Str::A::A(const std::string &x) : len{x.size()}, ptr{new char[Zof()]} {
    auto str = x.c_str();
    cpy(str);
}

Str::A::~A() {
    delete[] ptr;
}

Str::A::A(const A &x) : len{x.len}, ptr{new char[Zof()]} {
    cpy(x.ptr);
}

auto Str::A::operator=(const A &x) -> A& {
    len = x.len, ptr = new char[Zof()];
    cpy(x.ptr);
    return *this;
}

auto Str::A::split_first(char c) -> std::tuple<A, A> {
    /* find the char in the string */
    auto idx = fnd((char[]){c, 0});

    /* make the buffers */
    auto x = new char[idx+1];
    auto y = new char[len-idx-1];
    memmove(x, ptr, idx);             x[idx]=0;
    memmove(y, ptr+idx+2, len-idx-1); y[len-idx-2]=0;

    return {Str::A(x), Str::A(y)};
}

