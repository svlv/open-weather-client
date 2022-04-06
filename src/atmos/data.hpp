#pragma once

#include <optional>
#include <string>
#include <vector>

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

struct weather_data
{
  struct coord_t {
    double lon;
    double lat;
  };

  struct main_t {
    double temp;
    double feels_like;
    double temp_min;
    double temp_max;
    int pressure;
    int humidity;
  };

  struct sys_t {
    std::string country;
    int sunrise;
    int sunset;
  };

  coord_t coord;
  std::vector<weather_t> weather;
  std::string base;
  main_t main;
  int visibility;
  wind_t wind;
  clouds_t clouds;
  int dt;
  sys_t sys;
  int timezone;
  int id;
  std::string name;
  int cod;
};

struct forecast_data
{
  struct forecast_t
  {
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

    int dt;
    std::string dt_txt;
    main_t main;
    std::vector<weather_t> weather;
    clouds_t clouds;
    wind_t wind;
    int visibility;
    double pop;
  };

  int cnt; // A number of timestamps
  std::vector<forecast_t> list;
};
