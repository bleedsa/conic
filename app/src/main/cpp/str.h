#ifndef CONIC_STR_H
#define CONIC_STR_H

#include <cstring>
#include <string>
#include <sstream>
#include <iostream>

#include "u.h"

namespace Str {
    /* A is a static-width vector of char with a len and a ptr */
    struct A {
        S len;
        char *ptr;
        static constexpr const char *TAG = "Str::A";

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
            memmove(ptr, x, Zof());
        }

        /* convert to a c string */
        [[nodiscard]]
        inl auto c_str() const -> const char* {
            auto str = new char[Zof()];
            memmove(str, ptr, Zof());
            return str;
        }

        /* convert to a c++ string */
        inl auto str() -> std::string {
            std::stringstream ss;
            ss << ptr;
            return ss.str();
        }

        /* find the index of a string in the str */
        inl auto fnd(const char *str) -> long long {
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

        inl auto Zof() -> S {
            return Z(char)*(len+1);
        }

        inl Parser(const char *str) : len{strlen(str)}, ptr{new char[Zof()]} {
            memmove(ptr, str, Zof());
        }
        inl Parser(const std::string &str) : len{str.size()}, ptr{new char[Zof()]} {
            memmove(ptr, str.c_str(), Zof());
        }
        inl ~Parser(){delete[] ptr;}

        /* copy a parser into this parser */
        inl auto cpy(const Parser &x) -> void {
            len = x.len, ptr = new char[Zof()];
            memmove(ptr, x.ptr, Zof());
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
            auto endl = strstr(ptr, "\n");
            auto len = endl - ptr;
            auto buf = new char[len+1];
            memmove(buf, ptr, Z(char)*len);
            buf[len]=0;
            log_info(TAG, "ln(): buf: %s", buf);

            auto r = Str::A(buf);
            delete[] buf;
            ptr += len+1;
            return r;
        }
    };
}

/* parse an integer */
inl auto operator>>(Str::Parser x, S &r) -> Str::Parser {
    std::istringstream(x.ptr) >> r;
    x.ptr += std::to_string(r).length();
    return x;
}

#endif
