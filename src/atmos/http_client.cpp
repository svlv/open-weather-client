#include "http_client.hpp"

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <algorithm>
#include <cassert>

http_client::http_client() : m_sock(-1) {}

http_client::~http_client() {
    if (m_sock != -1) {
        close(m_sock);
    }
}

bool http_client::connect(std::string_view host, std::string_view port) {
    assert(m_sock == -1);

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    struct addrinfo *result;
    if (0 != getaddrinfo(host.data(), port.data(), &hints, &result)) {
        return false;
    }

    struct addrinfo *rp;
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        m_sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (m_sock == -1) {
            continue;
        }

        if (-1 != ::connect(m_sock, rp->ai_addr, rp->ai_addrlen)) {
            break;
        }

        close(m_sock);
        m_sock = -1;
    }

    freeaddrinfo(result);
    assert(m_sock != -1);

    return rp != NULL;
}

void http_client::write(std::string_view request) {
    assert(m_sock != -1);
    auto ret = ::send(m_sock, request.data(), request.size(), 0);
    assert(ret == request.size());
}

std::string http_client::read() {
    assert(m_sock != -1);

    char buf[1024];

    const auto read_to_buf = [&buf, this]() -> int {
        int valread = ::read(m_sock, buf, sizeof(buf) - 1);
        buf[valread] = '\0';
        return valread;
    };

    auto valread = read_to_buf();

    const auto* cnt_length = strstr(buf, "Content-Length: ");
    assert(cnt_length != nullptr);

    size_t length = 0;
    assert(1 == sscanf(cnt_length, "Content-Length: %u\r\n", &length));

    std::string content;
    content.resize(length);

    auto pos = std::find(buf, buf + valread, '{');
    assert(pos != buf + valread);

    std::copy(pos, buf + valread, content.begin());
    size_t sz = buf + valread - pos;

    while (true) {
        valread = read_to_buf();
        if (valread <= 0) {
            break;
        }
        std::copy(buf, buf + valread, content.begin() + sz);
        sz += valread;
    }

    assert(sz == content.size());

    return content;
}
