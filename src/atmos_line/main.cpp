#include "atmos/atmos.hpp"
#include "atmos/config.hpp"
#include "atmos/utils.hpp"
#include <iostream>
#include <clocale>

int main()
{
  auto data = get_current_weather();
  std::setlocale(LC_ALL, "en_US.UTF-8");
  std::wcout << get_weather_icon(data.weather[0].id) << L" " << to_int(to_celsium(data.main.temp)) << cels_deg_sym <<
    L" " << wind_icon << L" " << get_wind_dir_icon(data.wind.deg) << L" " 
    << to_int(data.wind.speed);
  if (data.wind.gust.has_value()) {
    std::wcout << L"-" << to_int(data.wind.gust.value());
  }
  std::wcout << L" m/s" << std::endl;

  return 0;
}
