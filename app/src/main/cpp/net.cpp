#include <sstream>
#include <vector>
#include <cassert>
#include <iostream>
#include <cstdio>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#include "u.h"
#include "R.h"
#include "net.h"

auto Net::parse_addr_port(const char *x) -> R<std::tuple<std::string, std::string>> {
    std::stringstream addr, port;
    char *ptr;
    S col /* colon idx */, i, L;

    log_info(TAG, "parse_addr_port(%s)", x);

    ptr = strchr((char*)x, ':');
    if (ptr == nullptr)
        [[unlikely]]
        return R<std::tuple<std::string, std::string>>("malformed address: no ':'");
    L = strlen(x);

    for (i = 0; x+i < ptr; i++) addr << x[i];
    for (i += 1; x[i] != 0; i++) port << x[i];

    auto tup = std::tuple<std::string, std::string>{addr.str(), port.str()};
    return R<std::tuple<std::string, std::string>>(tup);
}

auto Net::mksock(const char *addr_port) -> R<int> {
    int ret, sock;
    const char *addr, *port;
    struct addrinfo hints{}, *res, *p;
    bool connected;

    /* split up the addr port string */
    auto r = parse_addr_port((char*)addr_port);
    if (!r) return R<int>(r.err.c_str());
    auto [addr_str, port_str] = *r;

    addr = addr_str.c_str();
    port = port_str.c_str();

    log_info(TAG, "mksock(): connecting to %s:%s", addr, port);

    /* get address info */
    memset(&hints, 0, Z(hints));
    hints.ai_family = AF_UNSPEC; /* v4 or v6 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 6; /* IPROTO_TCP */
    if ((ret = getaddrinfo(addr, port, &hints, &res)) != 0) {
        return R<int>(str_fmt("getaddrinfo(): " << gai_strerror(ret)));
    }

    /* make socket */
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock == -1) R<int>(str_fmt("socket(): " << strerror(errno)));

    /* make connection */
    for (p = res, connected = false; p != nullptr; p = p->ai_next) {
        ret = connect(sock, p->ai_addr, p->ai_addrlen);
        if (ret >= 0) {
            connected = true;
            break;
        } else {
            log_err(TAG, "mksock(): failed to connect...retrying...");
        }
    }

    freeaddrinfo(res);

    return R<int>(sock);
}

Net::Http::Http(const char *addr) : addr{Str::A(addr)} {
    auto r = mkhttpsock(this->addr.c_str());
    if (!r) throw r.err.str();
    sock = *r;
}

Net::Http::Http(const std::string &addr) : addr{Str::A(addr)} {
    auto r = mkhttpsock(addr);
    if (!r) throw r.err.str();
    sock = *r;
}
Net::Http::~Http() {close(sock);}

inl auto mkhttpgetheader(
        const char *addr,
        const char *path,
        const char *query
) -> Str::A {
    std::stringstream ss;

    ss << "GET " << path << " Http/1.1\r\n";
    ss << "Host " << addr;
    if (!Str::is_empty(query)) ss << '?' << query << "\r\n";
    ss << "User-Agent: Conic/" << U::VERSION << "\r\n";
    ss << "Accept: */*";
    ss << "\r\n";

    auto str = ss.str();
    auto len = str.size();
    auto ptr = new char[len+1];
    auto cstr = str.c_str();
    memmove(ptr, cstr, Z(char) * (len+1));

    return Str::A(ptr);
}

auto Net::Http::send(Str::A *str) const -> R<int> {
    constexpr const char *TAG = "Net::Http::send";
    int sent, total = 0;
    auto ptr = str->ptr;
    auto len = str->len;

    log_info(TAG, "sending %zu bytes...", len);
    fflush(stdout);
    while (total < len) {
        if ((sent = ::send(sock, ptr, len-total, 0)) < 0)
            return err_errno<int>();
        total += sent;
    }
    log_info(TAG, "sent %d bytes.", total);

    return R<int>(total);
}

auto Net::Http::recv() -> R<Str::A> {
    constexpr const char *TAG = "Net::Http::recv()";
    constexpr S z = 2048;
    int got, total = 0;
    char chnk[z];
    std::stringstream ss;

    do {
        /* clear the chunk */
        memset(chnk, 0, z);
        log_info(TAG, "recv'ing %zu bytes...", z);

        /* perform the recv and add it to the buffer */
        if ((got = ::recv(sock, chnk, z-1, 0)) < 0) {
            err_errno<Str::A>();
        } else if (got < z) {
            /* incomplete chunk. add it to the buffer and exit */
            chnk[got-1] = 0;
            total += got;
            ss << chnk;
            log_info(TAG, "recv'd <%zu bytes; finishing. (%d total)", z, total);
            break;
        } else {
            /* add this chunk to the buffer and wait for the next one */
            log_info(TAG, "recv'd %d bytes.", got)
            chnk[z-1] = 0;
            total += got;
            ss << chnk;
        }
    } while (1);

    auto str = Str::A(ss.str());
    log_info(TAG, "returning body\n%s", str.ptr);

    return R<Str::A>(str);
}

inl auto parser_next_is_header(Str::Parser &prs) -> bool {
    auto next_nl = strstr(prs.ptr, "\r\n");
    auto next_colon = strstr(prs.ptr, ":");
    if (!next_nl || !next_colon || next_nl == prs.ptr || next_colon == prs.ptr) return false;
    return next_nl > next_colon;
}

auto Net::Http::parse_HttpResult(const Str::A &str) -> R<HttpResult> {
    constexpr const char *TAG = "Net::Http::parse_HttpResult";
    auto src = str.ptr;
    auto len = str.len;

    /* find information on the status line ie
     * HTTP/1.0 200 OK\r\n */
    auto status_ptr = strstr(src, " ")+1;
    if (!status_ptr || !*status_ptr)
        return err<HttpResult>(str_fmt("could not find \\r\\n in response"<<src));

    /* make a parser and get the status */
    auto prs = Str::Parser(status_ptr);
    S status;
    prs >> status;
    prs.skip_ln();

    /* parse the headers */
    std::vector<std::tuple<Str::A, Str::A>> headers;
    while (parser_next_is_header(prs)) headers.push_back(prs.ln().split_first(':'));

    log_info(TAG, "parsed status %zu", status);
    for (const auto &[k, v] : headers) log_info(TAG, "parsed header %s:%s", k.ptr, v.ptr);

    assert(false);
}

/* perform a GET requesst */
auto Net::Http::get(const char *path, const char *query) -> R<HttpResult> {
    auto a = addr.c_str();
    log_info(TAG, "get(): %s", a);
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