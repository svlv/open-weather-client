//#define BOOST_BEAST_USE_STD_STRING_VIEW
#include "http_client.hpp"
#include "data_parser.hpp"
#include "utils.hpp"
#include <iostream>
#include <clocale>
#include <cstdlib>

constexpr boost::string_view host = "api.openweathermap.org";
constexpr std::string_view port = "80";
constexpr std::string_view TARGET = "/data/2.5/weather";
constexpr std::string_view token = "e5b292ae2f9dae5f29e11499c2d82ece";
constexpr std::string_view city = "Minsk";
const int version = 10;

std::string_view env_token = std::getenv("OW_TOKEN");
std::string_view env_city = std::getenv("OW_CITY");

std::string make_target(std::string_view city, std::string_view token)
{
  std::ostringstream os;
  os << TARGET << "?q=" << city << "&appid=" << token;
  return os.str();
}

http::request<http::string_body> make_request(boost::string_view target, boost::string_view host)
{
  http::request<http::string_body> req{http::verb::get, target, version};
  boost::string_view str = http::to_string(http::field::host);
  req.set(boost::beast::http::field::host, host);
  req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
  return req;
}

int main()
{
  auto target = make_target(env_city.empty() ? city : env_city,
                            env_token.empty() ? token : env_token);
  auto request = make_request(target, host);
  http_client client;
  client.connect(std::string_view(host.data(), host.size()), port);
  client.write(request);
  auto answer = client.read();
  //std::cout << answer << std::endl;
  data_parser parser(answer);
  auto temp = parser.get_temp();
  auto weather_id = parser.get_weather_id();
  auto wind_deg = parser.get_wind_deg();
  auto wind_speed = parser.get_wind_speed();
  auto wind_gust = parser.get_wind_gust();

  std::setlocale(LC_ALL, "en_US.UTF-8");
  std::wcout << get_weather_icon(weather_id) << L"  " << to_int(temp) << cels_deg_sym <<
    L"  " << wind_icon << L" " << get_wind_dir_icon(wind_deg) << L" " 
    << to_int(wind_speed);
  if (wind_gust.has_value()) {
    std::wcout << L"-" << to_int(wind_gust.value());
  }
  std::wcout << L" m/s" << std::endl;

  return 0;
}

