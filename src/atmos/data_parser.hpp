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
    boost::property_tree::ptree _pt;
};
