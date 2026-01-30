#include <sstream>
#include <iostream>
#include <cstdio>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#include "u.h"
#include "J.h"
#include "R.h"
#include "net.h"

auto Net::parse_addr_port(const char *x) -> R<std::tuple<std::string, std::string>> {
    std::stringstream addr, port;
    char *ptr;
    S col /* colon idx */, i, L;

    ptr = strchr((char*)x, ':');
    if (ptr == nullptr)
        return R<std::tuple<std::string, std::string>>("malformed address: no ':'");
    col = (S)(ptr - x);
    L = strlen(x);

    for (i = 0; i < col; i++) addr << x[i];
    for (i = col+1; i < L; i++) port << x[i];

    auto tup = std::tuple<std::string, std::string>{addr.str(), port.str()};
    return R<std::tuple<std::string, std::string>>(tup);
}

auto Net::mksock(const char *addr_port) -> int {
    int ret, sock;
    const char *addr, *port;
    struct addrinfo hints{}, *res, *p;
    bool connected;

    /* split up the addr port string */
    auto r = parse_addr_port((char*)addr_port);
    if (!r) fatal("failed to parse address: %s", r.err.c_str());
    auto [addr_str, port_str] = *r;

    addr = addr_str.c_str();
    port = port_str.c_str();

    /* get address info */
    memset(&hints, 0, Z(hints));
    hints.ai_family = AF_UNSPEC; /* v4 or v6 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 6; /* IPROTO_TCP */
    if ((ret = getaddrinfo(addr, port, &hints, &res)) != 0) {
        fatal("getaddrinfo(): %s", gai_strerror(ret));
    }

    /* make socket */
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == -1) fatal("socket(): %s", strerror(errno));

    /* make connection */
    for (p = res, connected = false; p != nullptr; p = p->ai_next) {
        ret = connect(sock, p->ai_addr, p->ai_addrlen);
        if (ret >= 0) {
            connected = true;
            break;
        } else {
            std::cout << "failed to connect...retrying..." << std::endl;
        }
    }

    freeaddrinfo(res);

    return sock;
}

Net::Http::Http(const char *addr) : sock{mkhttpsock(addr)}, addr{Str::A(addr)} {}
Net::Http::Http(const std::string &addr) : sock{mkhttpsock(addr)}, addr{Str::A(addr)} {}
Net::Http::~Http() {close(sock);}

inl auto mkhttpgetheader(
        const char *addr,
        const char *path,
        const char *query
) -> Str::A {
    std::stringstream ss;

    ss << "GET " << path << " Http/1.1\r\n";
    ss << "Host " << addr << '?' << query << "\r\n";
    ss << "User-Agent: Conic/" << U::VERSION << "\r\n";
    ss << "\r\n";

    auto str = ss.str();
    auto len = str.size();
    auto ptr = new char[len+1];
    auto cstr = str.c_str();
    memmove(ptr, cstr, Z(char) * (len+1));

    return Str::A(ptr);
}

template<typename E>
inl auto err(int sock, const char *msg) -> R<E> {
    close(sock);
    return R<E>(msg);
}

template<typename E>
inl auto err_errno(int sock) -> R<E> {
    return err<E>(sock, strerror(errno));
}

auto Net::Http::send(Str::A *str) const -> R<int> {
    int sent, total = 0;
    auto ptr = str->ptr;
    auto len = str->len;

    std::cout << "sending " << len << "bytes...";
    fflush(stdout);
    while (total < len) {
        if ((sent = ::send(sock, ptr, len-total, 0)) < 0)
            return err_errno<int>(sock);
        total += sent;
    }
    std::cout << "ok " << total << std::endl;

    return R<int>(total);
}

auto Net::Http::recv() -> R<Str::A> {
    int got, total;
    constexpr auto z = 2048;
    char chnk[z];
    std::stringstream ss;

    do {
        /* clear the chunk */
        memset(chnk, 0, z);
        if ((got = ::recv(sock, chnk, z-1, 0)) < 0)
            break;
        else {
            chnk[z-1] = 0;
            total += got;
            ss << chnk;
        }
    } while (1);

    return R<Str::A>(Str::A(ss.str()));
}

auto Net::Http::parse_HttpResult(const Str::A &str) -> R<HttpResult> {
    /* TODO */
}

auto Net::Http::get(const char *path, const char *query) -> R<HttpResult> {
    auto a = this->addr.c_str();
    auto head = mkhttpgetheader(a, path, query);

    /* send the header */
    auto sent = send(&head);
    if (!sent) return R<HttpResult>(sent.err.c_str());

    /* recv the response */
    auto got = recv();
    if (!got) return R<HttpResult>(got.err.c_str());

    /* parse */
    auto res = parse_HttpResult(*got);
    if (!res) return R<HttpResult>(res.err.c_str());

    return res;
}


