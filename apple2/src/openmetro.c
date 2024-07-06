/*
 *
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <conio.h>

#include "fujinet-network.h"
#include "weatherdefs.h"
#include "weatherui.h"
#include "weatherdisp.h"
#include "openmetro.h"

extern UNITOPT unit_opt;
extern int	err;


char omurl[256];
char om_head[] = "N:https://api.open-meteo.com/v1/forecast?latitude=";
char om_lon[] = "&longitude=";

char om_tail_weather1[] ="&timezone=auto&timeformat=unixtime&current=relative_humidity_2m,weather_code,cloud_cover,surface_pressure";
char om_tail_weather2[] ="&current=temperature_2m,apparent_temperature,,wind_speed_10m,wind_direction_10m&hourly=dew_point_2m,visibility&forecast_hours=1";

char om_tail_forecast1[] ="&forecast_days=8&forecast_hours=1&timeformat=unixtime&daily=weather_code,temperature_2m_max,temperature_2m_min,sunrise,sunset";
char om_tail_forecast2[] ="&forecast_days=8&forecast_hours=1&daily=wind_speed_10m_max,wind_direction_10m_dominant,precipitation_sum,uv_index_max";

/* unit option string */
char *unit_str[] = {"&wind_speed_unit=ms", "&temperature_unit=fahrenheit&wind_speed_unit=mph"};

/* geocoding api */
char om_geocoding_head[] = "N:https://geocoding-api.open-meteo.com/v1/search?name=";
char om_geocoding_tail[] = "&count=1&language=en&format=json";

char city[40];

bool om_geocoding(LOCATION *loc, char *city) {
	strcpy(omurl, om_geocoding_head);
	strcat(omurl, city);
	strcat(omurl, om_geocoding_tail);

    network_open(omurl, OPEN_MODE_READ, OPEN_TRANS_NONE);
    err = network_json_parse(omurl);
    handle_err("open metro geocoding parse");

    err = network_json_query(omurl, "/results/0/name", loc->city);
    handle_err("name query");
	if (strlen(loc->city) == 0) {
		return(false);
	}
    err = network_json_query(omurl, "/results/0/longitude", loc->lon);
    //handle_err("lon query");

    err = network_json_query(omurl, "/results/0/latitude", loc->lat);
    //handle_err("lat query");

    err = network_json_query(omurl, "/results/0/country_code", loc->countryCode);
    //handle_err("country code query");

	network_close(omurl);

	return(true);
}

//
// setup Open-Metro URL
//
void setup_omurl(LOCATION *loc, char *param) {
	strcpy(omurl, om_head);
    strcat(omurl, loc->lat);
    strcat(omurl, om_lon);
    strcat(omurl, loc->lon);
    strcat(omurl, param);
	strcat(omurl, unit_str[unit_opt]);
}
//
//
//
void get_om_info(LOCATION *loc, WEATHER *wi, FORECAST *fc) {
	char querybuf[LINE_LEN];

// weather 1 query
	setup_omurl(loc, om_tail_weather1);

    network_open(omurl, OPEN_MODE_READ, OPEN_TRANS_NONE);
    err = network_json_parse(omurl);
    handle_err("parse");

//	city name, country code
	strcpy(wi->name, loc->city);
	strcpy(wi->country, loc->countryCode);

//  date & time
    err = network_json_query(omurl, "/current/time", querybuf);
    //handle_err("dt query");
	wi->td = atol(querybuf);
//  timezone(offset) 
    err = network_json_query(omurl, "/utc_offset_seconds", querybuf);
    //handle_err("tz query");
	wi->tz = atol(querybuf);
// timezone
    err = network_json_query(omurl, "/timezone", wi->timezone);
    //handle_err("timezone query");
//  pressure
    err = network_json_query(omurl, "/current/surface_pressure", wi->pressure);
    //handle_err("pressure query");
//  humidity
    err = network_json_query(omurl, "/current/relative_humidity_2m", wi->humidity);
    //handle_err("humidity query");
// weather code (icon)
    err = network_json_query(omurl, "/current/weather_code", querybuf);
    //handle_err("icon query");
	wi->icon = atoi(querybuf);
//  clouds
    err = network_json_query(omurl, "/current/cloud_cover", wi->clouds);
    //handle_err("wind_deg query");

	network_close(omurl);	// of weather1


// weather 2 query
	setup_omurl(loc, om_tail_weather2);

    network_open(omurl, OPEN_MODE_READ, OPEN_TRANS_NONE);
    err = network_json_parse(omurl);
    handle_err("parse");

//  temperatur
    err = network_json_query(omurl, "/current/temperature_2m", wi->temp);
    //handle_err("temp query");
//  feels_like
    err = network_json_query(omurl, "/current/apparent_temperature", wi->feels_like);
    //handle_err("feels-like query");
//  dew_point
    err = network_json_query(omurl, "/hourly/dew_point_2m/0", wi->dew_point);
    //handle_err("dew-point query");
//  visibility
    err = network_json_query(omurl, "/hourly/visibility/0", wi->visibility);
    //handle_err("visibility query");
//  wind_speed
    err = network_json_query(omurl, "/current/wind_speed_10m", wi->wind_speed);
    //handle_err("wind_speed query");
//  wind_deg
    err = network_json_query(omurl, "/current/wind_direction_10m", wi->wind_deg);
    //handle_err("wind_deg query");

	network_close(omurl);	// of weather2

//	forecast
//  part 1
	setup_omurl(loc, om_tail_forecast1);

    network_open(omurl, OPEN_MODE_READ, OPEN_TRANS_NONE);
    err = network_json_parse(omurl);
    handle_err("forecast 1 parse");

	set_forecast1(fc);
	network_close(omurl);	// of forecast part 1

//  copy today's sunrise/sunset from forecat data to weather data
	wi->sunrise = fc->day[0].sunrise; 
	wi->sunset = fc->day[0].sunset; 

//  part 2
	setup_omurl(loc, om_tail_forecast2);

    network_open(omurl, OPEN_MODE_READ, OPEN_TRANS_NONE);
    err = network_json_parse(omurl);
    handle_err("forecast 2 parse");

	set_forecast2(fc);
	network_close(omurl);	// of forecast part 2
}
//
// set forecast data part1
//
void set_forecast1(FORECAST *fc) {
	char	i;
	char querybuf[40];
	char prbuf[40];

	for (i=0; i<=7; i++) {
// date & time
		sprintf(querybuf, "/daily/time/%d", i);
	    err = network_json_query(omurl, querybuf, prbuf);
    	handle_err("fc dt  query");
		fc->day[i].td = atol(prbuf);
// sunrise 
		sprintf(querybuf, "/daily/sunrise/%d", i);
	    err = network_json_query(omurl, querybuf, prbuf);
    	//handle_err("fc sunrise  query");
		fc->day[i].sunrise = atol(prbuf);
// sunset 
		sprintf(querybuf, "/daily/sunset/%d", i);
	    err = network_json_query(omurl, querybuf, prbuf);
    	//handle_err("fc sunrise  query");
		fc->day[i].sunset = atol(prbuf);
// temp min
		sprintf(querybuf, "/daily/temperature_2m_min/%d", i);
	    err = network_json_query(omurl, querybuf, fc->day[i].temp_min);
    	//handle_err("fc temp min  query");
// temp max
		sprintf(querybuf, "/daily/temperature_2m_max/%d", i);
	    err = network_json_query(omurl, querybuf, fc->day[i].temp_max);
    	//handle_err("fc temp max  query");
// icon
		sprintf(querybuf, "/daily/weather_code/%d", i);
	    err = network_json_query(omurl, querybuf, prbuf);
    	//handle_err("fc icon  query");
		fc->day[i].icon = atoi(prbuf);
	}
}
//
// set forecast data part 2
//
void set_forecast2(FORECAST *fc) {
	char	i;
	char querybuf[40];

	for (i=0; i<=7; i++) {
// precipitation sum
		sprintf(querybuf, "/daily/precipitation_sum/%d", i);
	    err = network_json_query(omurl, querybuf, fc->day[i].precipitation_sum);
    	//handle_err("fc precipitaion sum query");
// uv index  max
		sprintf(querybuf, "/daily/uv_index_max/%d", i);
	    err = network_json_query(omurl, querybuf, fc->day[i].uv_index_max);
    	//handle_err("fc precipitaion sum query");
// wind  speed
		sprintf(querybuf, "/daily/wind_speed_10m_max/%d", i);
	    err = network_json_query(omurl, querybuf, fc->day[i].wind_speed);
    	//handle_err("fc wind speed  query");
// wind  deg
		sprintf(querybuf, "/daily/wind_direction_10m_dominant/%d", i);
	    err = network_json_query(omurl, querybuf, fc->day[i].wind_deg);
    	//handle_err("fc wind deg  query");
	}
}
