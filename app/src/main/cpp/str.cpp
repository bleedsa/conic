#include "str.h"

Str::A::A(const char *x) : len{strlen(x)}, ptr{new char[len]} {
    cpy(x);
}

Str::A::A(const std::string &x) : len{x.size()}, ptr{new char[len]} {
    cpy(x.c_str());
}

Str::A::~A() {
    delete[] ptr;
}

Str::A::A(const A &x) : len{x.len}, ptr{new char[len]} {
    cpy(x.ptr);
}

auto Str::A::operator=(const A &x) -> const A& {
    len = x.len, ptr = new char[len];
    cpy(x.ptr);
    return *this;
}