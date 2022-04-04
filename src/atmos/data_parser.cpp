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

template <>
data::coord_t deserialize(const boost::property_tree::ptree& pt)
{
  return {
    pt.get<double>("lon"),
    pt.get<double>("lat")
  };
}

template <>
data::main_t deserialize(const boost::property_tree::ptree& pt)
{
  return {
    pt.get<double>("temp"),
    pt.get<double>("feels_like"),
    pt.get<double>("temp_min"),
    pt.get<double>("temp_max"),
    pt.get<int>("pressure"),
    pt.get<int>("humidity")
  };
}

template <>
data::sys_t deserialize(const boost::property_tree::ptree& pt)
{
  return {
    pt.get<std::string>("country"),
    pt.get<int>("sunrise"),
    pt.get<int>("sunset")
  };
}

data_parser::data_parser(const std::string& data)
{
  std::stringstream ss(data);
  boost::property_tree::read_json(ss, _pt);
}

data data_parser::parse() const
{
  return {
    deserialize<data::coord_t>(_pt.get_child("coord")),
    deserialize<std::vector<weather_t>>(_pt.get_child("weather")),
    _pt.get<std::string>("base"),
    deserialize<data::main_t>(_pt.get_child("main")),
    _pt.get<int>("visibility"),
    deserialize<wind_t>(_pt.get_child("wind")),
    deserialize<clouds_t>(_pt.get_child("clouds")),
    _pt.get<int>("dt"),
    deserialize<data::sys_t>(_pt.get_child("sys")),
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
