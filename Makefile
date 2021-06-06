all: open-weather-client

open-weather-client: open_weather_client.o
	gcc -o open-weather-client open_weather_client.o -lm

open_weather_client.o: open_weather_client.c
	gcc -c open_weather_client.c

clean:
	rm open_weather_client.o open_weather_client
