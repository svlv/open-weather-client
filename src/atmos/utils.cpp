#include "utils.hpp"
#include "config.hpp"
#include <boost/beast/version.hpp>

std::string make_target(std::string_view city, std::string_view token)
{
  std::ostringstream os;
  os << target << "?q=" << city << "&appid=" << token;
  return os.str();
}

http::request<http::string_body> make_request(std::string_view target, std::string_view host)
{
  http::request<http::string_body> req{http::verb::get, target, version};
  req.set(boost::beast::http::field::host, host);
  req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
  return req;
}

std::wstring_view get_wind_dir_icon(int deg) {
  if (deg >= 337)
    return L"↓";
  else if (deg >= 293)
    return L"↘";
  else if (deg >= 247)
    return L"→";
  else if (deg >= 203)
    return L"↗";
  else if (deg >= 157)
    return L"↑";
  else if (deg >= 113)
    return L"↖";
  else if (deg >= 67)
    return L"←";
  else if (deg >= 23)
    return L"↙";
  return L"↓";
}

std::wstring_view get_weather_icon(int id)
{
  if (id >= 200 && id <= 232)
    return L"🌩";
  else if (id >=300 && id <= 321)
    return L"🌧";
  else if (id >= 500 && id <= 531)
    return L"🌧";
  else if (id >= 600 && id <= 622)
    return L"❄️";
  else if (id >= 700 && id <= 781)
    return L"🌫️";
  else if (id == 800)
    return L"☀️";
  else if (id >= 801 && id <= 804)
    return L"☁️";
  return L"";
}
