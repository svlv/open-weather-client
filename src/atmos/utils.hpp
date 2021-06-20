#pragma once

#include <cmath>
#include <boost/beast/http.hpp>

namespace http = boost::beast::http;

constexpr std::string_view host = "api.openweathermap.org";
constexpr std::string_view port = "80";
constexpr std::string_view TARGET = "/data/2.5/weather";
constexpr std::string_view token = "e5b292ae2f9dae5f29e11499c2d82ece";
constexpr std::string_view city = "Minsk";
const int version = 10;

constexpr std::wstring_view cels_deg_sym = L"℃";
constexpr std::wstring_view wind_icon = L"💨";

static inline double to_celsium(double kelvin) {
  return kelvin - 273.15;
}

static inline int to_int(double val) {
  return static_cast<int>(std::round(val));
}

std::wstring_view get_wind_dir_icon(int deg);
std::wstring_view get_weather_icon(int id);

std::string make_target(std::string_view city, std::string_view token);
http::request<http::string_body> make_request(std::string_view target, std::string_view host);

