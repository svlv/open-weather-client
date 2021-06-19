#include <boost/property_tree/ptree.hpp>

class data_parser
{
  public:
    explicit data_parser(const std::string& data);
    int get_weather_id() const;
    double get_temp() const;

    // Wind
    double get_wind_speed() const;
    int get_wind_deg() const;
    std::optional<double> get_wind_gust() const;

  private:
    boost::property_tree::ptree _pt;
};

