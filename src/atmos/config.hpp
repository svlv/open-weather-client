#pragma once

#include <string_view>

constexpr std::string_view host = "api.openweathermap.org";
constexpr std::string_view port = "80";
constexpr std::string_view target = "/data/2.5/weather";

constexpr std::string_view target2 = "data/2.5/forecast/hourly";
constexpr std::string_view host2 = "pro.openweathermap.org";

constexpr std::string_view token = "e5b292ae2f9dae5f29e11499c2d82ece";
constexpr std::string_view city = "Minsk";
const int version = 10;

constexpr std::wstring_view cels_deg_sym = L"℃";
constexpr std::wstring_view wind_icon = L"💨";
