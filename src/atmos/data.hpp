#pragma once

#include <optional>
#include <string>
#include <vector>

struct data
{
  struct coord_t {
    double lon;
    double lat;
  };
  coord_t coord;
  struct weather_t {
    int id;
    std::string main;
    std::string description;
    std::string icon;
  };
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
  struct wind_t {
    double speed;
    int deg;
    std::optional<double> gust;
  };
  int visibility;
  wind_t wind;
  struct clouds_t {
    int all;
  };
  clouds_t clouds;
  int dt;
  struct sys_t {
//    int type;
//    int id;
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

