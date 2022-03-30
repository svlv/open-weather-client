#pragma once

#include <string_view>
#include <vector>

struct field_value
{
    field_value(std::string_view fld, std::string_view val);
    std::string to_string() const;

    std::string_view field;
    std::string_view value;
};

class query_string
{
public:
    query_string() = default;

    template <typename... Args>
    query_string(std::string_view field, std::string_view value, Args... args)
    {
        add(field, value, args...);
    }

    template <typename... Args>
    void add(std::string_view field, std::string_view value, Args... args)
    {
        add(field, value);
        add(args...);
    }

    void add(std::string_view field, std::string_view value);

    std::string to_string() const;
private:
    std::vector<field_value> _queries;
};
