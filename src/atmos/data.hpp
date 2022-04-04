#pragma once

#include <optional>
#include <string>
#include <vector>

#include <boost/property_tree/ptree.hpp>
namespace pt = boost::property_tree;

struct weather_t
{
  int id;
  std::string main;
  std::string description;
  std::string icon;
};

struct clouds_t
{
  int all;
};

struct wind_t
{
  double speed;
  int deg;
  std::optional<double> gust;
};

struct data
{
  struct coord_t {
    double lon;
    double lat;
  };
  coord_t coord;
  std::vector<weather_t> weather;
  std::string base;
  struct main_t {
    double temp;
    double feels_like;
    double temp_min;
    double temp_max;
    int pressure;
    int humidity;
  };
  main_t main;
  int visibility;
  wind_t wind;
  clouds_t clouds;
  int dt;
  struct sys_t {
    std::string country;
    int sunrise;
    int sunset;
  };
  sys_t sys;
  int timezone;
  int id;
  std::string name;
  int cod;
};

struct forecast_data
{
  int cnt; // A number of timestamps
  struct forecast_t
  {
    int dt;
    std::string dt_txt;
    struct main_t {
      double temp;
      double feels_like;
      double temp_min;
      double temp_max;
      int pressure;
      int sea_level;
      int grnd_level;
      int humidity;
    };
    main_t main;
    std::vector<weather_t> weather;
    clouds_t clouds;
    wind_t wind;
    int visibility;
    double pop;
  };
  std::vector<forecast_t> list;
};
