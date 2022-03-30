#include "query_string.hpp"
#include <sstream>

const char delimiter = '&';

field_value::field_value(std::string_view fld, std::string_view val)
    : field(fld), value(val)
{
}

std::string field_value::to_string() const
{
    std::ostringstream os;
    os << field << '=' << value;
    return os.str();
}

void query_string::add(std::string_view field, std::string_view value)
{
    _queries.emplace_back(field, value);
}

std::string query_string::to_string() const
{
    std::string res;
    for (const auto& query : _queries)
    {
        if (!res.empty())
        {
            res += delimiter;
        }
        res += query.to_string();
    }
    return res;
}
