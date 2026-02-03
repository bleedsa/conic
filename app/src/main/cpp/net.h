#ifndef CONIC_NET_H
#define CONIC_NET_H

#include <tuple>
#include <string>
#include <sstream>
#include <map>
#include <utility>
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

    using Headers = std::map<Str::A, Str::A>;

    struct HttpResult {
        int status;
        S bodyL;
        char *body;
        Headers headers;

        inl HttpResult(int status, Headers headers, const char *body)
            : status{status}
            , bodyL{strlen(body)}
            , body{new char[bodyL+1]}
            , headers{std::move(headers)}
        {
            memmove((void*)this->body, body, bodyL);
	    this->body[bodyL]=0;
        }

        inl HttpResult(const HttpResult &x)
            : status{x.status}
            , bodyL{x.bodyL}
            , body{new char[bodyL+1]}
            , headers{x.headers}
        {
            memmove((void *) this->body, x.body, bodyL);
	    this->body[bodyL]=0;
        }

	inl ~HttpResult() {
		delete[] body;
	}

        inl auto operator=(const HttpResult &x) -> HttpResult& {
            status = x.status;
	    bodyL = x.bodyL+1;
	    body = new char[bodyL+1];
	    headers = x.headers;
            memmove((void*)this->body, x.body, bodyL);
	    this->body[bodyL]=0;
            return *this;
        }

        inl auto str() const -> std::string {
            std::stringstream ss;
	    ss << "=================================" << std::endl;
            ss << "HTTP STATUS: " << status << std::endl;
	    ss << "=================================" << std::endl;
            for (auto &[k, v] : headers)
		    ss << k.ptr << ": " << v.ptr << std::endl;
	    ss << "=================================" << std::endl;
	    ss << body << std::endl;
            return ss.str();
        }

        inl auto c_str() const -> const char* {
            auto s = str();
            S len = s.size() + 1;
            auto ptr = new char[len];
            memmove(ptr, s.c_str(), len);
            return ptr;
        }
    };

    struct Http {
        static constexpr const char *TAG = "Net::Http";
        Str::A addr;
        int sock;

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
