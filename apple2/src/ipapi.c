/*
 * # Weather application for FujiNet                     
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
#include "weatherdisp.h"
#include "ipapi.h"


extern int	err;

char ip_url[] = "N:http://ip-api.com/json/?fields=status,city,countryCode,lon,lat";

//char city[40];

//
// get location info from ip 
// returns true: call is success
//         false:     is not success
//
bool get_location(LOCATION *loc) {
	char	buf[10];

	network_open(ip_url, OPEN_MODE_READ, OPEN_TRANS_NONE);
    err = network_json_parse(ip_url);
    handle_err("parse");

	err = network_json_query(ip_url, "/status", buf);
    handle_err("status query");
	if (strcmp(buf, "success") != 0) {
		network_close(ip_url);
		return (false);
	}

	err = network_json_query(ip_url, "/city", loc->city);
    handle_err("city query");

	err = network_json_query(ip_url, "/countryCode", loc->countryCode);
    handle_err("country query");

	err = network_json_query(ip_url, "/lon", loc->lon);
    handle_err("lon query");

	err = network_json_query(ip_url, "/lat", loc->lat);
    handle_err("lat query");

	network_close(ip_url);

	return(true);
}
