#include <stdio.h>
#include <sys/socket.h>
#include <string.h> // memset
#include <netdb.h> // gethostbyname
#include <unistd.h> // close
#include <arpa/inet.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <wchar.h>
#include <locale.h>

#define DEFAULT_CITY "Minsk"
#define DEFAULT_TOKEN "e5b292ae2f9dae5f29e11499c2d82ece"

enum val_type {
  time_type = 1,
  float_type = 2,
  str_type = 3,
  int_type,
  wchar_type
};

void get_value(const char* response, const char* key,
               enum val_type type, void* result) {
  char* beg = strstr(response, key);
  if (beg != NULL) {
    beg += strlen(key) + 1;
    char* end = strchr(beg, ',');
    char value[256];
    memset(value, 0, sizeof(value));
    memcpy(value, beg, end - beg);

    switch (type) {
      case time_type:
        *(time_t*)result = atoi(value);
        break;
      case float_type:
        *(float*)result = atof(value);
        break;
      case str_type:
        memcpy(result, value + 1, strlen(value) - 2);
        break;
      case int_type:
        *(int*)result = atoi(value);
      default:
        break;
    }
  }
}

void replace(wchar_t* str, const wchar_t* key,
             enum val_type type, void* value) {
  wchar_t* beg = wcsstr(str, key);
  if (beg == NULL) {
    return;
  }
  wchar_t buff[256];
  wmemset(buff, 0, 256);
  size_t sz = beg - str;
  wmemcpy(buff, str, sz);
  switch (type) {
    case int_type:
      swprintf(buff + sz, 256, L"%i", *(int*)value);
      break;
    case wchar_type:
      swprintf(buff + sz, 256, L"%lc", *(wchar_t*)value);
      break;
    case time_type:
    case float_type:
    case str_type:
    default:
      break;
  }
  beg += wcslen(key) ;
  wmemcpy(buff + wcslen(buff), beg, wcslen(beg));
  wmemset(str, 0, wcslen(str));
  wmemcpy(str, buff, wcslen(buff));
}

float to_celsium(float kelvin) {
  return kelvin - 273.15;
}

wchar_t get_wind_dir_icon(int deg) {
  if (deg >= 337)
    return L'↓';
  else if (deg >= 293)
    return L'↘';
  else if (deg >= 247)
    return L'→';
  else if (deg >= 203)
    return L'↗';
  else if (deg >= 157)
    return L'↑';
  else if (deg >= 113)
    return L'↖';
  else if (deg >= 67)
    return L'←';
  else if (deg >= 23)
    return L'↙';
  return L'↓';
}

wchar_t get_weaither_icon(int id)
{
  if (id >= 200 && id <= 232)
    return L'🌩';
  else if (id >=300 && id <= 321)
    return L'🌧';
  else if (id >= 500 && id <= 531)
    return L'🌧';
  else if (id >= 600 && id <= 622)
    return L'❄️';
  else if (id >= 700 && id <= 781)
    return L'🌫️';
  else if (id == 800)
    return L'☀️';
  else if (id >= 801 && id <= 804)
    return L'☁️';
  return L'';
}

int main(int argc, char *argv[]) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    fprintf(stderr, "Error creating a socket: %s\n", strerror(errno));
    return 1;
  }

  struct hostent* server = gethostbyname("api.openweathermap.org");
  if (!server) {
    fprintf(stderr, "The host is not found: %s\n", strerror(errno));
    close(sock);
    return 1;
  }

  struct sockaddr_in sin;
  memset(&sin, 0, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(80);
  memcpy(&sin.sin_addr.s_addr, server->h_addr, server->h_length);

  if (connect(sock, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
    fprintf(stderr, "Failed to connect to the server: %s\n", strerror(errno));
    close(sock);
    return 1;
  }

  char* city = getenv("OW_CITY");
  if (city == NULL) {
    city = DEFAULT_CITY;
  }
  char* token = getenv("OW_TOKEN");
  if (token == NULL) {
    token = DEFAULT_TOKEN;
  }

  char msg[256];
  sprintf(msg, "GET /data/2.5/weather?q=%s&appid=%s HTTP/1.0\r\n\r\n", city, token);
  int bytes = write(sock, msg, strlen(msg));
  if (bytes < 0) {
    fprintf(stderr, "Failed to send request: %s\n", strerror(errno));
    close(sock);
    return 1;
  }

  char response[4096];
  memset(response, 0, sizeof(response));
  bytes = read(sock, response, sizeof(response) - 1);
  if (bytes < 0) {
    fprintf(stderr, "Failed to read response: %s\n", strerror(errno));
    close(sock);
    return 1;
  }

  close(sock);

#ifdef DEBUG
  printf("%s\n", response);
#endif

  char weather[256];
  memset(&weather, 0, sizeof(weather));
  get_value(response, "\"main\"", str_type, &weather);

  time_t sunrise = 0;
  get_value(response, "\"sunrise\"", time_type, &sunrise);
  struct tm sunrise_tm = *localtime(&sunrise);

  time_t sunset = 0;
  get_value(response, "\"sunset\"", time_type, &sunset);
  struct tm sunset_tm = *localtime(&sunset);

  float temp = 0.0f;
  get_value(response, "\"temp\"", float_type, &temp);

  float wind_speed = 0.0;
  get_value(response, "\"speed\"", float_type, &wind_speed);

  int deg = 0;
  get_value(response, "\"deg\"", int_type, &deg);

  if (argc == 1) {
    printf("Weather: %s\n"
           "Sunrise: %i:%iam\n"
           "Sunset: %i:%iam\n"
           "Temperature: %i℃\n"
           "Wind speed: %i m/s\n",
           weather,
           sunrise_tm.tm_hour, sunrise_tm.tm_min,
           sunset_tm.tm_hour, sunset_tm.tm_min,
           (int)round(to_celsium(temp)),
           (int)round(wind_speed));
  } else if (argc == 2) {
    setlocale(LC_ALL, "en_US.UTF-8");
    wchar_t result[256];
    wmemset(result, 0, 256);
    mbstowcs(result, argv[1], 256);

    int tmp = (int)round(to_celsium(temp));
    replace(result, L"%temp", int_type, &tmp);
    wchar_t wind_dir_icon = get_wind_dir_icon(deg);
    replace(result, L"%winddiricon", wchar_type, &wind_dir_icon);
    int wind = (int)round(wind_speed);
    replace(result, L"%wind", int_type, &wind);
    wprintf(L"%ls\n", result);
  }
  return 0;
}

