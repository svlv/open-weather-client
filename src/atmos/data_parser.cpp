#include "data_parser.hpp"
#include "utils.hpp"
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/property_tree/json_parser.hpp>

template <class T> T deserialize(const boost::property_tree::ptree& pt);

template <>
wind_t deserialize(const boost::property_tree::ptree& pt)
{
  const auto& gust = pt.get_optional<double>("gust");
  return {
    pt.get<double>("speed"),
    pt.get<int>("deg"),
    gust.has_value() ? gust.get() : std::optional<double>()
  };
}

template <>
forecast_data::forecast_t::main_t deserialize(const boost::property_tree::ptree& pt)
{
  return {
      pt.get<double>("temp"),
      pt.get<double>("feels_like"),
      pt.get<double>("temp_min"),
      pt.get<double>("temp_max"),
      pt.get<int>("pressure"),
      pt.get<int>("sea_level"),
      pt.get<int>("grnd_level"),
      pt.get<int>("humidity")
  };
}

template <>
weather_t deserialize(const boost::property_tree::ptree& pt)
{
  return {
    pt.get<int>("id"),
    pt.get<std::string>("main"),
    pt.get<std::string>("description"),
    pt.get<std::string>("icon")
  };
}

template <>
std::vector<weather_t> deserialize(const boost::property_tree::ptree& pt)
{
  std::vector<weather_t> vec;
  for (const auto& weather : pt) {
    vec.emplace_back(deserialize<weather_t>(weather.second));
  }
  return vec;
}

template<>
clouds_t deserialize(const boost::property_tree::ptree& pt)
{
  return {
    pt.get<int>("all")
  };
}

template <>
forecast_data::forecast_t deserialize(const boost::property_tree::ptree& pt)
{
  return {
    pt.get<int>("dt"),
    pt.get<std::string>("dt_txt"),
    deserialize<forecast_data::forecast_t::main_t>(pt.get_child("main")),
    deserialize<std::vector<weather_t>>(pt.get_child("weather")),
    deserialize<clouds_t>(pt.get_child("clouds")),
    deserialize<wind_t>(pt.get_child("wind")),
    pt.get<int>("visibility"),
    pt.get<double>("pop")
  };
}

template <>
std::vector<forecast_data::forecast_t> deserialize(const boost::property_tree::ptree& pt)
{
  std::vector<forecast_data::forecast_t> list;
  for (const auto& forecast : pt) {
    list.emplace_back(deserialize<forecast_data::forecast_t>(forecast.second));
  }
  return list;
}

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
    deserialize<std::vector<weather_t>>(_pt.get_child("weather")),
    get_base(),
    get_main(),
    _pt.get<int>("visibility"),
    deserialize<wind_t>(_pt.get_child("wind")),
    deserialize<clouds_t>(_pt.get_child("clouds")),
    _pt.get<int>("dt"),
    get_sys(),
    _pt.get<int>("timezone"),
    _pt.get<int>("id"),
    _pt.get<std::string>("name"),
    _pt.get<int>("cod")
  };
}

forecast_data data_parser::parse_forecast() const
{
  return {
    _pt.get<int>("cnt"),
    deserialize<std::vector<forecast_data::forecast_t>>(_pt.get_child("list"))
  };
}
