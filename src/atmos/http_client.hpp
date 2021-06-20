#pragma once

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;

class http_client
{
  public:
    http_client();
    void connect(std::string_view host, std::string_view port);
    void write(const http::request<http::string_body>& req);
    std::string read();
  private:
    boost::asio::io_context _ioc;
    boost::beast::tcp_stream _stream;
};

