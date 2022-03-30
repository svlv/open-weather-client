#include "atmos.hpp"
#include "http_client.hpp"
#include "data_parser.hpp"
#include "utils.hpp"
#include <cstdlib>
#include "config.hpp"

data get_current_weather()
{
  char* env_token = std::getenv("OW_TOKEN");
  char* env_city = std::getenv("OW_CITY");
  auto target = make_target(env_city ? env_city : city,
                            env_token ? env_token : token);
  auto request = make_request(target, host);
  http_client client;
  client.connect(host, port);
  client.write(request);
  auto response = client.read();
  data_parser parser(response);
  return parser.parse();
}
