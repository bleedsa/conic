#ifndef CONIC_STR_H
#define CONIC_STR_H

#include <cstring>
#include <string>
#include <sstream>
#include <iostream>
#include <functional>

#include "u.h"

namespace Str {
    /* is this string empty? */
    inl auto is_empty(const char *x) -> bool {
        return x != nullptr && x[0] == 0;
    }

    /* A is a static-width vector of char with a len and a ptr */
    struct A {
        S len;
        char *ptr;
        static constexpr const char *TAG = "Str::A";

        inl A() : len{0}, ptr{nullptr} {}

        explicit A(const char *x);
        explicit A(const std::string &x);
        ~A();

        A(const A &x);
        A &operator=(const A &x);

        /* sizeof */
        [[nodiscard]]
        inl auto Zof() const -> S {
            return Z(char)*(1+len);
        }

        /* copy a string into the ptr */
        inl auto cpy(const char *x) -> void {
            if (ptr) [[likely]] memmove(ptr, x, Zof());
        }

        /* convert to a c string */
        [[nodiscard]]
        inl auto c_str() const -> const char* {
            if (!ptr) [[unlikely]] {
                auto e = new char[1];
                e[0]=0;
                return e;
            } else [[likely]] {
                auto str = new char[Zof()];
                memmove(str, ptr, Zof());
                return str;
            }
        }

        /* convert to a c++ string */
        inl auto str() -> std::string {
            if (ptr) [[likely]] {
                std::stringstream ss;
                ss << ptr;
                return ss.str();
            } else [[unlikely]] {
                return std::string();
            }
        }

        /* find the index of a string in the str */
        inl auto fnd(const char *str) -> signed long long {
            if (!ptr) [[unlikely]] return -1;
            /* find */
		    auto f = strstr(ptr, str);
            /* safety */
		    if (!f || f==str) [[unlikely]] return -1;
            return f - ptr;
        }

        /* split the first strings separated by char c */
	    std::tuple<A, A> split_first(char c);
    };

    /* parse stuff out of a string tape */
    struct Parser {
        S len;
        char *ptr;
        static constexpr const char *TAG = "Str::Parser";

        /* sizof the buffer in ptr */
        inl auto Zof() -> S {
            return Z(char)*(len+1);
        }

        inl Parser(const char *str) : len{strlen(str)}, ptr{(char*)str} {}
        inl Parser(const std::string &str)
	    : len{str.size()}
	    , ptr{(char*)str.c_str()}
	{}
        inl ~Parser(){}

        /* copy a parser into this parser */
        inl auto cpy(const Parser &x) -> void {
            len = x.len, ptr = x.ptr;
        }
        inl Parser(const Parser &x) {cpy(x);}
        inl auto operator=(const Parser &x) -> Parser& {cpy(x);return *this;}

        /* skip i chars */
        inl auto skip(S i) -> void {
            ptr += i;
        }

        /* go back i chars */
        inl auto back(S i) -> void {
            ptr -= i;
        }

        /* skip over a line */
        inl auto skip_ln() -> void {
            auto endl = strstr(ptr, "\n");
            ptr = endl+1;
        }

        /* parse out a line */
        inl auto ln() -> Str::A {
            /* find the endl */
            auto endl = strstr(ptr, "\n");
            auto len = endl - ptr;

            /* move the line into a buffer */
            auto buf = new char[len+1];
            memmove(buf, ptr, Z(char)*len);
            buf[len]=0;

            /* cleanup and return */
            auto r = Str::A(buf);
            delete[] buf;
            ptr += len+1;
            return r;
        }
    };
}

/* parse an S using stl */
inl auto operator>>(Str::Parser x, S &r) -> Str::Parser {
    std::istringstream(x.ptr) >> r;
    x.ptr += std::to_string(r).length();
    return x;
}

/* parse an int using stl */
inl auto operator>>(Str::Parser x, int &r) -> Str::Parser {
    std::istringstream(x.ptr) >> r;
    x.ptr += std::to_string(r).length();
    return x;
}

namespace std {
    template<>
    class less<Str::A> {
    public:
        inl auto operator()(const Str::A &x, const Str::A &y) const -> bool {
            return strcmp(x.ptr, y.ptr) < 0;
        }
    };
}

#endif
