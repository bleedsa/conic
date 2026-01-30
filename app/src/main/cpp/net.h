#ifndef CONIC_NET_H
#define CONIC_NET_H

#include <tuple>
#include <string>
#include <sstream>

#include "u.h"
#include "R.h"

namespace Net {
    R<std::tuple<std::string, std::string>> parse_addr_port(const char *x);

    int mksock(const char *addr_port);

    inl auto mksock(const std::string &x) -> int {
        return mksock(x.c_str());
    }

    inl auto mkhttpsock(const char *addr) -> int {
        std::stringstream ss;
        ss << addr << ":80";
        return mksock(ss.str());
    }

    inl auto mkhttpsock(const std::string &x) -> int {
        return mkhttpsock(x.c_str());
    }

    struct HttpResult {
        int status;
        S bodyL;
        const char *body;

        inl HttpResult(int status, const char *body)
            : status{status}
            , bodyL{strlen(body)}
            , body{new char[bodyL]}
        {
            memmove((void*)this->body, body, Z(char)*bodyL);
        }

        inl HttpResult(const HttpResult &x)
            : status{x.status}
            , bodyL{x.bodyL}
            , body{new char[bodyL]}
        {
            memmove((void *) this->body, body, Z(char)*bodyL);
        }

        inl auto operator=(const HttpResult &x) -> const HttpResult& {
            status = x.status, bodyL = x.bodyL, body = new char[bodyL];
            memmove((void*)this->body, body, Z(char)*bodyL);
            return *this;
        }
    };

    struct Http {
        int sock;
        Str::A addr;

        explicit Http(const char *addr);
        explicit Http(const std::string &addr);
        ~Http();

        R<int> send(Str::A *str) const;
        R<Str::A> recv();
        R<HttpResult> parse_HttpResult(const Str::A &str);
        R<HttpResult> get(const char *path, const char *query);
    };
}

#endif
