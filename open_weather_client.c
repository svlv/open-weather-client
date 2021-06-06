#include <stdio.h>
#include <sys/socket.h>
#include <string.h> // memset
#include <netdb.h> // gethostbyname
#include <unistd.h> // close
#include <arpa/inet.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>

#define DEFAULT_CITY "Minsk"
#define DEFAULT_TOKEN "e5b292ae2f9dae5f29e11499c2d82ece"

enum val_type {
  time_type = 1,
  float_type = 2,
  str_type = 3
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
    }
  }
}

float to_celsium(float kelvin) {
  return kelvin - 273.15;
}

int main() {
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
  //printf("%s\n", response);

  char weather[256];
  memset(&weather, 0, sizeof(weather));
  get_value(response, "\"main\"", str_type, &weather);
  printf("Weather: %s\n", weather);

  time_t sunrise = 0;
  get_value(response, "\"sunrise\"", time_type, &sunrise);
  struct tm* sunrise_tm = localtime(&sunrise);
  printf("Sunrise: %i:%iam\n", sunrise_tm->tm_hour, sunrise_tm->tm_min);

  time_t sunset = 0;
  get_value(response, "\"sunset\"", time_type, &sunset);
  struct tm* sunset_tm = localtime(&sunset);
  printf("Sunset: %i:%iam\n", sunset_tm->tm_hour, sunset_tm->tm_min);

  float temp = 0.0f;
  get_value(response, "\"temp\"", float_type, &temp);
  printf("Temperature: %.2f℃\n", to_celsium(temp));

  float wind_speed = 0.0;
  get_value(response, "\"speed\"", float_type, &wind_speed);
  printf("Wind speed: %.2f m/s\n", wind_speed);

  close(sock);
  return 0;
}

