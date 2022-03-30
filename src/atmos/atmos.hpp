#pragma once
//#define BOOST_BEAST_USE_STD_STRING_VIEW

#include "data.hpp"

data get_current_weather(double lat, double lon, std::string_view token);

forecast_data get_5_day_3_hour_forecast(double lat, double lon, std::string_view token);
