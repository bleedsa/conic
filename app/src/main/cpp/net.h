#ifndef CONIC_NET_H
#define CONIC_NET_H

#include <tuple>
#include <string>
#include <sstream>
#include <unistd.h>

#include "u.h"
#include "R.h"

namespace Net {
    constexpr const char *TAG = "Net";

    R<std::tuple<std::string, std::string>> parse_addr_port(const char *x);
    R<int> mksock(const char *addr_port);
    inl auto mksock(const std::string &x) -> R<int> {
        return mksock(x.c_str());
    }

    inl auto mkhttpsock(const char *addr) -> R<int> {
        std::stringstream ss;
        ss << addr << ":80";
        return mksock(ss.str());
    }
    inl auto mkhttpsock(const std::string &x) -> R<int> {
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

        inl auto operator=(const HttpResult &x) -> HttpResult& {
            status = x.status, bodyL = x.bodyL, body = new char[bodyL];
            memmove((void*)this->body, body, Z(char)*bodyL);
            return *this;
        }
    };

    struct Http {
        Str::A addr;
        int sock;
        static constexpr const char *TAG = "Net::Http";

        Http(const char *addr);
        Http(const std::string &addr);
        ~Http();

        R<int> send(Str::A *str) const;
        R<Str::A> recv();
        R<HttpResult> parse_HttpResult(const Str::A &str);
        R<HttpResult> get(const char *path, const char *query);

        template<typename E>
        inl auto err(const char *msg) const -> R<E> {
            close(sock);
            log_err(TAG, "error: %s", msg);
            return R<E>(msg);
        }

        template<typename E>
        inl auto err(const std::string &x) const -> R<E> {
            return err<E>(x.c_str());
        }

        template<typename E>
        inl auto err_errno() const -> R<E> {
            return err<E>(strerror(errno));
        }
    };
}

#endif
