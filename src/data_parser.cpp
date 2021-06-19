#include "data_parser.hpp"
#include "utils.hpp"
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/property_tree/json_parser.hpp>
#include <iostream>
data_parser::data_parser(const std::string& data)
{
  std::stringstream ss(data);
  boost::property_tree::read_json(ss, _pt);
}

int data_parser::get_weather_id() const
{
  for (const boost::property_tree::ptree::value_type& val : _pt.get_child("weather"))
  {
    return val.second.get<int>("id");
  }
  return 0;
}

double data_parser::get_temp() const
{
  return to_celsium(_pt.get<double>("main.temp"));
}

double data_parser::get_wind_speed() const
{
  return _pt.get<double>("wind.speed");
}

std::optional<double> data_parser::get_wind_gust() const
{
  try {
    auto value = _pt.get<double>("wind.gust");
    return value;
  } catch (boost::property_tree::ptree_bad_path&) {
    return {};
  }
}

int data_parser::get_wind_deg() const
{
  return _pt.get<int>("wind.deg");
}

