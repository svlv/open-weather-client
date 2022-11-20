#pragma once

#include <optional>
#include <string>
#include <vector>

#include "reflection.hpp"

struct weather_t
{
  START_REFLECTABLE_MEMBERS();
  REFLECTABLE_MEMBER(int, id);
  REFLECTABLE_MEMBER(std::string, main);
  REFLECTABLE_MEMBER(std::string, description);
  REFLECTABLE_MEMBER(std::string, icon);
  END_REFLECTABLE_MEMBERS();
};

struct clouds_t
{
  START_REFLECTABLE_MEMBERS();
  REFLECTABLE_MEMBER(int, all);
  END_REFLECTABLE_MEMBERS();
};

struct wind_t
{
  START_REFLECTABLE_MEMBERS();
  REFLECTABLE_MEMBER(double, speed);
  REFLECTABLE_MEMBER(int, deg);
  REFLECTABLE_MEMBER(std::optional<double>, gust);
  END_REFLECTABLE_MEMBERS();
};

struct coord_t {
  START_REFLECTABLE_MEMBERS();
  REFLECTABLE_MEMBER(double, lon);
  REFLECTABLE_MEMBER(double, lat);
  END_REFLECTABLE_MEMBERS();
};

struct weather_data
{
  struct main_t {
    START_REFLECTABLE_MEMBERS();
    REFLECTABLE_MEMBER(double, temp);
    REFLECTABLE_MEMBER(double, feels_like);
    REFLECTABLE_MEMBER(double, temp_min);
    REFLECTABLE_MEMBER(double, temp_max);
    REFLECTABLE_MEMBER(int, pressure);
    REFLECTABLE_MEMBER(int, humidity);
    END_REFLECTABLE_MEMBERS();
  };

  struct sys_t {
    START_REFLECTABLE_MEMBERS();
    REFLECTABLE_MEMBER(std::string, country);
    REFLECTABLE_MEMBER(int, sunrise);
    REFLECTABLE_MEMBER(int, sunset);
    END_REFLECTABLE_MEMBERS();
  };

  START_REFLECTABLE_MEMBERS();
  REFLECTABLE_MEMBER(coord_t, coord);
  REFLECTABLE_MEMBER(std::vector<weather_t>, weather);
  REFLECTABLE_MEMBER(std::string, base);
  REFLECTABLE_MEMBER(main_t, main);
  REFLECTABLE_MEMBER(int, visibility);
  REFLECTABLE_MEMBER(wind_t, wind);
  REFLECTABLE_MEMBER(clouds_t, clouds);
  REFLECTABLE_MEMBER(int, dt);
  REFLECTABLE_MEMBER(sys_t, sys);
  REFLECTABLE_MEMBER(int, timezone);
  REFLECTABLE_MEMBER(int, id);
  REFLECTABLE_MEMBER(std::string, name);
  REFLECTABLE_MEMBER(int, cod);
  END_REFLECTABLE_MEMBERS();
};

struct forecast_data
{
  struct forecast_t
  {
    struct main_t {
      START_REFLECTABLE_MEMBERS();
      REFLECTABLE_MEMBER(double, temp);
      REFLECTABLE_MEMBER(double, feels_like);
      REFLECTABLE_MEMBER(double, temp_min);
      REFLECTABLE_MEMBER(double, temp_max);
      REFLECTABLE_MEMBER(int, pressure);
      REFLECTABLE_MEMBER(int, sea_level);
      REFLECTABLE_MEMBER(int, grnd_level);
      REFLECTABLE_MEMBER(int, humidity);
      END_REFLECTABLE_MEMBERS();
    };

    START_REFLECTABLE_MEMBERS();
    REFLECTABLE_MEMBER(int, dt);
    REFLECTABLE_MEMBER(std::string, dt_txt);
    REFLECTABLE_MEMBER(main_t, main);
    REFLECTABLE_MEMBER(std::vector<weather_t>, weather);
    REFLECTABLE_MEMBER(clouds_t, clouds);
    REFLECTABLE_MEMBER(wind_t, wind);
    REFLECTABLE_MEMBER(int, visibility);
    REFLECTABLE_MEMBER(double, pop);
    END_REFLECTABLE_MEMBERS();
  };

  START_REFLECTABLE_MEMBERS();
  REFLECTABLE_MEMBER(int, cnt); // A number of timestamps
  REFLECTABLE_MEMBER(std::vector<forecast_t>, list);
  END_REFLECTABLE_MEMBERS();
};

struct air_pollution_data
{
  struct air_pollution_t {
    struct main_t {
      START_REFLECTABLE_MEMBERS();
      REFLECTABLE_MEMBER(int, aqi);
      END_REFLECTABLE_MEMBERS();
    };

    struct components_t {
      START_REFLECTABLE_MEMBERS();
      REFLECTABLE_MEMBER(double, co);
      REFLECTABLE_MEMBER(double, no);
      REFLECTABLE_MEMBER(double, no2);
      REFLECTABLE_MEMBER(double, o3);
      REFLECTABLE_MEMBER(double, so2);
      REFLECTABLE_MEMBER(double, pm2_5);
      REFLECTABLE_MEMBER(double, pm10);
      REFLECTABLE_MEMBER(double, nh3);
      END_REFLECTABLE_MEMBERS();
    };

    START_REFLECTABLE_MEMBERS();
    REFLECTABLE_MEMBER(main_t, main);
    REFLECTABLE_MEMBER(components_t, components);
    REFLECTABLE_MEMBER(int, dt);
    END_REFLECTABLE_MEMBERS();
  };

  START_REFLECTABLE_MEMBERS();
  REFLECTABLE_MEMBER(coord_t, coord);
  REFLECTABLE_MEMBER(std::vector<air_pollution_t>, list);
  END_REFLECTABLE_MEMBERS();
};
