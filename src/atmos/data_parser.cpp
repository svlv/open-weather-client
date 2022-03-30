#include "data_parser.hpp"
#include "utils.hpp"
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/property_tree/json_parser.hpp>

data_parser::data_parser(const std::string& data)
{
  std::stringstream ss(data);
  boost::property_tree::read_json(ss, _pt);
}

data::coord_t data_parser::get_coord() const
{
  const auto& coord = _pt.get_child("coord");
  return {coord.get<double>("lon"), coord.get<double>("lat")};
}

std::vector<weather_t> data_parser::get_weather() const
{
  std::vector<weather_t> res;
  for (const auto& weather : _pt.get_child("weather")) {
    res.emplace_back(weather_t {
        weather.second.get<int>("id"),
        weather.second.get<std::string>("main"),
        weather.second.get<std::string>("description"),
        weather.second.get<std::string>("icon")
    });
  }
  return res;
}

std::string data_parser::get_base() const
{
  return _pt.get<std::string>("base");
}

data::main_t data_parser::get_main() const
{
  const auto& main = _pt.get_child("main");
  return {
    main.get<double>("temp"),
    main.get<double>("feels_like"),
    main.get<double>("temp_min"),
    main.get<double>("temp_max"),
    main.get<int>("pressure"),
    main.get<int>("humidity")
  };
}

int data_parser::get_visibility() const
{
  return _pt.get<int>("visibility");
}

wind_t data_parser::get_wind() const
{
  const auto& wind = _pt.get_child("wind");
  const auto& gust = wind.get_optional<double>("gust");
  return {
    wind.get<double>("speed"),
    wind.get<int>("deg"),
    gust.has_value() ? gust.get() : std::optional<double>()
  };
}

clouds_t data_parser::get_clouds() const
{
  const auto& clouds = _pt.get_child("clouds");
  return { clouds.get<int>("all") };
}

int data_parser::get_dt() const
{
  return _pt.get<int>("dt");
}

data::sys_t data_parser::get_sys() const
{
  const auto& sys = _pt.get_child("sys");
  return {
    //sys.get<int>("type"),
    //sys.get<int>("id"),
    sys.get<std::string>("country"),
    sys.get<int>("sunrise"),
    sys.get<int>("sunset")
  };
}

int data_parser::get_timezone() const
{
  return _pt.get<int>("timezone");
}

int data_parser::get_id() const
{
  return _pt.get<int>("id");
}

std::string data_parser::get_name() const
{
  return _pt.get<std::string>("name");
}

int data_parser::get_cod() const
{
  return _pt.get<int>("cod");
}

data data_parser::parse() const
{
  return {
    get_coord(),
    get_weather(),
    get_base(),
    get_main(),
    get_visibility(),
    get_wind(),
    get_clouds(),
    get_dt(),
    get_sys(),
    get_timezone(),
    get_id(),
    get_name(),
    get_cod()
  };
}

std::vector<forecast_data::forecast_t> data_parser::parse_forecast_list() const
{
  std::vector<forecast_data::forecast_t> list;
  for (const auto& elem : _pt.get_child("list")) {
    const auto& main = elem.second.get_child("main");
    forecast_data::forecast_t::main_t mn{
        main.get<double>("temp"),
        main.get<double>("feels_like"),
        main.get<double>("temp_min"),
        main.get<double>("temp_max"),
        main.get<int>("pressure"),
        main.get<int>("sea_level"),
        main.get<int>("grnd_level"),
        main.get<int>("humidity")
    };

    std::vector<weather_t> wthr;
    for (const auto& weather : elem.second.get_child("weather")) {
      wthr.emplace_back(weather_t {
          weather.second.get<int>("id"),
          weather.second.get<std::string>("main"),
          weather.second.get<std::string>("description"),
          weather.second.get<std::string>("icon")
      });
    }

    const auto& clouds = elem.second.get_child("clouds");
    clouds_t clds{ clouds.get<int>("all") };

    const auto& wind = elem.second.get_child("wind");
    const auto& gust = wind.get_optional<double>("gust");
    wind_t wnd {
      wind.get<double>("speed"),
      wind.get<int>("deg"),
      gust.has_value() ? gust.get() : std::optional<double>()
    };

    list.emplace_back(forecast_data::forecast_t {
      elem.second.get<int>("dt"),
      elem.second.get<std::string>("dt_txt"),
      mn, wthr, clds, wnd,
      elem.second.get<int>("visibility"),
      elem.second.get<double>("pop")
    });

  }
  return list;
}

forecast_data data_parser::parse_forecast() const
{
  return {
    40,
    parse_forecast_list()
  };
}
