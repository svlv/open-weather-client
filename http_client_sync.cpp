#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <iostream>
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;       // from <boost/beast/http.hpp>
namespace net = boost::asio;        // from <boost/asio.hpp>
using tcp = net::ip::tcp;           // from <boost/asio/ip/tcp.hpp>


int main(int argc, char** argv)
{
    try
    {
        auto const host = "api.openweathermap.org";
        auto const port = "80";
        auto const target = "/data/2.5/weather";
        int version = 10;

        // The io_context is required for all I/O
        net::io_context ioc;

        // These objects perform our I/O
        tcp::resolver resolver(ioc);
        beast::tcp_stream stream(ioc);

        // Look up the domain name
        auto const results = resolver.resolve(host, port);

        // Make the connection on the IP address we get from a lookup
        stream.connect(results);

        // Set up an HTTP GET request message
        
        std::string default_city = "Minsk";
        std::string default_token = "e5b292ae2f9dae5f29e11499c2d82ece";
        std::ostringstream os;
        os << "/data/2.5/weather" << "?q=" << default_city << "&appid=" << default_token;
        http::request<http::string_body> req{http::verb::get, os.str(), version};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

        // Send the HTTP request to the remote host
        http::write(stream, req);

        // This buffer is used for reading and must be persisted
        beast::flat_buffer buffer;

        // Declare a container to hold the response
        http::response<http::string_body> res;

        // Receive the HTTP response
        http::read(stream, buffer, res);

        // Write the message to standard out
        std::cout << res.body() << std::endl;

        boost::property_tree::ptree pt;
        std::stringstream ss(res.body());
        boost::property_tree::read_json(ss, pt);
        float temp = pt.get<float>("main.temp");
        std::cout << temp << std::endl;
        // Gracefully close the socket
        beast::error_code ec;
        stream.socket().shutdown(tcp::socket::shutdown_both, ec);

        // not_connected happens sometimes
        // so don't bother reporting it.
        //
        if(ec && ec != beast::errc::not_connected)
            throw beast::system_error{ec};

        // If we get here then the connection is closed gracefully
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

