#pragma once

#include <cmath>

static inline double to_celsium(double kelvin) {
  return kelvin - 273.15;
}

static inline int to_int(double val) {
  return static_cast<int>(std::round(val));
}

static inline std::wstring_view get_wind_dir_icon(int deg) {
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

static inline std::wstring_view get_weather_icon(int id)
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

constexpr std::wstring_view cels_deg_sym = L"℃";
constexpr std::wstring_view wind_icon = L"💨";
