#pragma once

#include "data.hpp"

weather_data get_current_weather(
    double lat,
    double lon,
    std::string_view token);

forecast_data get_5_day_3_hour_forecast(
    double lat,
    double lon,
    std::string_view token);
