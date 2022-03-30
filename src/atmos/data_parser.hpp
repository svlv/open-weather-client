#pragma once

#include <boost/property_tree/ptree.hpp>
#include "data.hpp"

class data_parser
{
  public:
    explicit data_parser(const std::string& data);
    data parse() const;
    forecast_data parse_forecast() const;

  private:
    data::coord_t get_coord() const;
    std::vector<weather_t> get_weather() const;
    std::string get_base() const;
    data::main_t get_main() const;
    int get_visibility() const;
    wind_t get_wind() const;
    clouds_t get_clouds() const;
    int get_dt() const;
    data::sys_t get_sys() const;
    int get_timezone() const;
    int get_id() const;
    std::string get_name() const;
    int get_cod() const;
    std::vector<forecast_data::forecast_t> parse_forecast_list() const;

  private:
    boost::property_tree::ptree _pt;
};
