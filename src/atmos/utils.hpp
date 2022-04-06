#pragma once

#include <cmath>
#include <string_view>

static inline double to_celsium(double kelvin) {
  return kelvin - 273.15;
}

static inline int to_int(double val) {
  return static_cast<int>(std::round(val));
}

wchar_t get_wind_dir_icon(int deg);

std::wstring_view get_weather_icon(int id);
