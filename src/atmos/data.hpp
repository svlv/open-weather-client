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
  //wind_t() = default;
  //wind_t(const pt::ptree& tree);

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

/*
    {
      "dt": 1649095200,
      "main": {
        "temp": 281.25,
        "feels_like": 277.77,
        "temp_min": 281.25,
        "temp_max": 281.25,
        "pressure": 1011,
        "sea_level": 1011,
        "grnd_level": 1011,
        "humidity": 88,
        "temp_kf": 0
      },
      "weather": [
        {
          "id": 804,
          "main": "Clouds",
          "description": "overcast clouds",
          "icon": "04d"
        }
      ],
      "clouds": {
        "all": 100
      },
      "wind": {
        "speed": 6.62,
        "deg": 267,
        "gust": 14.25
      },
      "visibility": 10000,
      "pop": 0.86,
      "sys": {
        "pod": "d"
      },
      "dt_txt": "2022-04-04 18:00:00"
    }
  ],
*/

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
