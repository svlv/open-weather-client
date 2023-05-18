#include "atmos.hpp"
#include "deserialize.hpp"
#include "http_client.hpp"
#include "query_string.hpp"

#include <boost/beast/version.hpp>
#include <cstdio>

constexpr std::string_view HOST = "api.openweathermap.org";
constexpr std::string_view PORT = "80";

namespace
{
std::string make_request(std::string_view path, std::string_view token, double lat, double lon)
{
  char lat_str[16];
  sprintf(lat_str, "%.4f", lat);

  char lon_str[16];
  sprintf(lon_str, "%.4f", lon);

  query_string queries("lat", lat_str, "lon", lon_str, "appid", token);

  std::string request(256, '\0');
  int sz = sprintf(request.data(), "GET %s?%s HTTP/1.0\r\n\r\n", path.data(),
                   queries.to_string().c_str());
  request.resize(sz);

  return request;
}
} // namespace

weather_data get_current_weather(double lat, double lon, std::string_view token)
{
  http_client client;
  client.connect(HOST, PORT);

  const std::string_view path = "/data/2.5/weather";
  client.write(make_request(path, token, lat, lon));

  return deserialize<weather_data>(client.read());
}

forecast_data get_5_day_3_hour_forecast(double lat, double lon, std::string_view token)
{
  http_client client;
  client.connect(HOST, PORT);

  const std::string_view path = "/data/2.5/forecast";
  client.write(make_request(path, token, lat, lon));

  return deserialize<forecast_data>(client.read());
}

air_pollution_data get_air_pollution(double lat, double lon, std::string_view token)
{
  http_client client;
  client.connect(HOST, PORT);

  const std::string_view path = "/data/2.5/air_pollution";
  client.write(make_request(path, token, lat, lon));

  return deserialize<air_pollution_data>(client.read());
}
