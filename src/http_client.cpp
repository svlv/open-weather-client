#include "http_client.hpp"


http_client::http_client() : _stream(_ioc) {}

void http_client::connect(std::string_view host, std::string_view port)
{
  boost::asio::ip::tcp::resolver resolver(_ioc);
  const auto results = resolver.resolve(host, port);
  _stream.connect(results);
}

void http_client::write(const http::request<http::string_body>& req)
{
  http::write(_stream, req);
}

std::string http_client::read()
{
  boost::beast::flat_buffer buffer;
  http::response<http::string_body> res;
  http::read(_stream, buffer, res);
  return res.body();
}

