#include "atmos.hpp"
#include "http_client.hpp"
#include "data_parser.hpp"
#include "utils.hpp"
#include <cstdlib>
std::string_view env_token = std::getenv("OW_TOKEN");
std::string_view env_city = std::getenv("OW_CITY");

data get_current_weather()
{
  auto target = make_target(env_city.empty() ? city : env_city,
                            env_token.empty() ? token : env_token);
  auto request = make_request(target, host);
  http_client client;
  client.connect(host, port);
  client.write(request);
  auto response = client.read();
  data_parser parser(response);
  return parser.parse();
}

