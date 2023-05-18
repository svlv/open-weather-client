#pragma once

#include <string_view>
#include <string>

class http_client
{
  public:
    http_client();
    ~http_client();
    bool connect(std::string_view host, std::string_view port);
    void write(std::string_view request);
    std::string read();
  private:
    int m_sock;
};
