#pragma once

#include <boost/property_tree/ptree.hpp>
#include "data.hpp"

class data_parser
{
  public:
    explicit data_parser(const std::string& data);
    data parse() const;

  private:
    data::coord_t get_coord() const;
    std::vector<data::weather_t> get_weather() const;
    std::string get_base() const;
    data::main_t get_main() const;
    int get_visibility() const;
    data::wind_t get_wind() const;
    data::clouds_t get_clouds() const;
    int get_dt() const;
    data::sys_t get_sys() const;
    int get_timezone() const;
    int get_id() const;
    std::string get_name() const;
    int get_cod() const;

  private:
    boost::property_tree::ptree _pt;
};
