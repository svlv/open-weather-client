#pragma once

#include <cmath>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;

std::string make_target(std::string_view city, std::string_view token);

http::request<http::string_body> make_request(std::string_view target, std::string_view host);

static inline double to_celsium(double kelvin) {
  return kelvin - 273.15;
}

static inline int to_int(double val) {
  return static_cast<int>(std::round(val));
}

std::wstring_view get_wind_dir_icon(int deg);

std::wstring_view get_weather_icon(int id);
