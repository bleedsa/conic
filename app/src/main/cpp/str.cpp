#include "str.h"

Str::A::A(const char *x) : len{strlen(x)}, ptr{x ? new char[Zof()] : nullptr} {
    cpy(x);
}

Str::A::A(const std::string &x) : len{x.size()}, ptr{new char[Zof()]} {
    auto str = x.c_str();
    cpy(str);
}

Str::A::~A() {
    delete[] ptr;
}

Str::A::A(const A &x) : len{x.len}, ptr{x.ptr ? new char[Zof()] : nullptr} {
    cpy(x.ptr);
}

auto Str::A::operator=(const A &x) -> A& {
    len = x.len, ptr = x.ptr ? new char[Zof()] : nullptr;
    cpy(x.ptr);
    return *this;
}

auto Str::A::split_first(char c) -> std::tuple<A, A> {
    /* safety */
    [[unlikely]]
    if (!ptr) throw str_fmt("split_first("<<c<<") on nullptr");

    /* find the char in the string */
    auto idx = fnd((char[]){c, 0});

    /* make the buffers */
    auto x = new char[idx+1], y = new char[len-idx-1];
    /* move */
    memmove(x, ptr, idx);             /* null */ x[idx]=0;
    memmove(y, ptr+idx+2, len-idx-1); /* null */ y[len-idx-2]=0;

    /* wrap */
    std::tuple<Str::A, Str::A> ret = {Str::A(x), Str::A(y)};
    delete[] x;delete[] y;
    return ret;
}

