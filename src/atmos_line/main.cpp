#include "atmos/atmos.hpp"
#include "atmos/config.hpp"
#include "atmos/utils.hpp"
#include <iostream>
#include <clocale>
#include <string_view>

#include<boost/program_options/options_description.hpp>
#include<boost/program_options/parsers.hpp>
#include<boost/program_options/variables_map.hpp>

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
  double lat = 0.0;
  double lon = 0.0;
  std::string token;

  po::options_description desc("Atmos usage");
  desc.add_options()
      ("lat", po::value<double>(&lat)->default_value(51.5072), "Geographical coordinate - latitude")
      ("lon", po::value<double>(&lon)->default_value(0.1276), "Geographical coordinate - longitude")
      ("token", po::value<std::string>(&token)->default_value("0dcf521f731dd46db31533ed4494c04c"), "Your unique API key")
      ("get-current-weather", "Get current weather for your location on Earth (London is default)")
      ("get-forecast", "Get 5 day/3 hour forecast");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);    

  if (vm.count("get-current-weather"))
  {
    auto data = get_current_weather(lat, lon, token);

    const auto cels = to_int(to_celsium(data.main.temp));
    const auto wind_speed = to_int(data.wind.speed);

    printf("%s %d°C %dm/s", data.weather[0].description, cels, wind_speed);
  }

  if (vm.count("get-forecast"))
  {
    auto data = get_5_day_3_hour_forecast(lat, lon, token);
    for (const auto& elem : data.list)
    {
      std::cout <<  elem.dt_txt << " " << to_int(to_celsium(elem.main.temp)) << std::endl;
    }
  }

  return 0;
}
