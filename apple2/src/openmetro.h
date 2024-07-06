#ifndef	OPENMETRO_H
#define OPENMETRO_H
#include <stdbool.h>

// function
bool get_location(LOCATION *loc);

// OpenMetro 
void setup_omurl();
char *time_str(char *buf);
bool om_geocoding(LOCATION *loc, char *city);
void get_om_info(LOCATION *loc, WEATHER *wi, FORECAST *fc);
void set_forecast1(FORECAST *fc);
void set_forecast2(FORECAST *fc);
#endif
