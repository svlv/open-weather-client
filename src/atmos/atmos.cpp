#include "atmos.hpp"
#include "http_client.hpp"
#include "data_parser.hpp"
#include "utils.hpp"
#include <cstdlib>
#include "config.hpp"
#include "query_string.hpp"
#include <cstdio>
#include <boost/beast/version.hpp>
#include <iostream>

constexpr std::string_view HOST = "api.openweathermap.org";
constexpr std::string_view PORT = "80";

namespace
{
http::request<http::string_body> make_request(std::string_view path, std::string_view token, double lat, double lon)
{
  char lat_str[16];
  sprintf(lat_str, "%.4f", lat);

  char lon_str[16];
  sprintf(lon_str, "%.4f", lon);

  query_string queries("lat", lat_str, "lon", lon_str, "appid", token);

  char full_path[256];
  sprintf(full_path, "%s?%s", path.data(), queries.to_string().c_str());

  http::request<http::string_body> req{http::verb::get, full_path, 10};
  req.set(boost::beast::http::field::host, host);
  req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

  return req;
}
} // namespace

data get_current_weather(double lat, double lon, std::string_view token)
{
  http_client client;
  client.connect(HOST, PORT);

  const std::string_view path = "/data/2.5/weather";
  client.write(make_request(path, token, lat, lon));

  data_parser parser(client.read());
  return parser.parse();
}

forecast_data get_5_day_3_hour_forecast(double lat, double lon, std::string_view token)
{
  http_client client;
  client.connect(HOST, PORT);

  const std::string_view path = "/data/2.5/forecast";
  client.write(make_request(path, token, lat, lon));

  data_parser parser(client.read());
  return parser.parse_forecast();
}
