#include "utils.hpp"

wchar_t get_wind_dir_icon(int deg) {
  if (deg >= 337)
    return L'↓';
  else if (deg >= 293)
    return L'↘';
  else if (deg >= 247)
    return L'→';
  else if (deg >= 203)
    return L'↗';
  else if (deg >= 157)
    return L'↑';
  else if (deg >= 113)
    return L'↖';
  else if (deg >= 67)
    return L'←';
  else if (deg >= 23)
    return L'↙';
  return L'↓';
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

std::string_view get_aqi_name(int aqi)
{
  if (aqi == 1)
    return "Good";
  else if (aqi == 2)
    return "Fair";
  else if (aqi == 3)
    return "Moderate";
  else if (aqi == 4)
    return "Poor";
  else if (aqi == 5)
    return "Very Poor";
  return "Unknown";
}
