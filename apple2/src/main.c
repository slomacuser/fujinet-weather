/**
 * @brief   weather
 * @author  Shunichi Kitahara
 * @license gpl v. 3
 * @verbose main
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "weatherdefs.h"
#include "weatherui.h"
#include "ipapi.h"
#include "openmeteo.h"
#include "weatherdisp.h"

LOCATION loc;
LOCATION current;
WEATHER  wi;
FORECAST fc;
char	current_screen;

typedef enum command {
	COM_REFRESH,
	COM_WEATHER,
	COM_FORECAST,
	COM_NONE
} COMMAND;

enum unit_option unit_opt = METRIC;
char weather_menu[] = "N)apoved O)svezi E)note L)okacija I)zhod         ";
char *forecast_menu[3] = {"???", " N)aprej  V)reme                 ",
                         " P)ojdi nazaj  V)reme                 "};

void main(void)
{
	bool	quit = false;
	char	forecast_page;
	char	ch;
	COMMAND com = COM_REFRESH;

	disp_message("     prenašam lokacijske podatke...");
	get_location(&loc);
	current = loc;
	forecast_page = 0;

	while (!quit) {
		switch (com) {
			case COM_REFRESH:		// refresh
				disp_message("     prenašam lokacijske podatke...");
				get_om_info(&loc, &wi, &fc);
				current_screen = 0;
			case COM_WEATHER:		// weather
				disp_weather(&wi);
				disp_menu(weather_menu);
				break;
			case COM_FORECAST:		// forecast
				disp_forecast(&fc, forecast_page);
				disp_menu(forecast_menu[forecast_page]);
				break;
			default:
				;
		}
		ch = get_keyin();
		if (current_screen == 1) {			// weather
			switch (ch) {
				case 'o':
				case 'O':
					com = COM_REFRESH;
					break;
				case 'e':
				case 'E':
					unit_opt = (unit_opt ==  METRIC) ? IMPERIAL : METRIC;
					com = COM_REFRESH;
					break;
				case 'l':
				case 'L':
					change_location(&loc);
					com = COM_REFRESH;
					break;
				case 'i':
				case 'I':
					quit = true;
					break;
				case 'n':
				case 'N':
					com = COM_FORECAST;
					forecast_page = 1;
					break;
				default:
					com = COM_NONE;
			}
		}
		else {								// forecast
			switch (ch) {
			case 'v':
			case 'V':
				com = COM_WEATHER;
				break;
			case 'n':
			case 'N':
				com = COM_FORECAST;
				forecast_page = 2;
				break;
			case 'p':
			case 'P':
				com = COM_FORECAST;
				forecast_page = 1;
				break;
			default:
				com = COM_NONE;
			}
		}
	}
	clear_display();
	exit(0);
}
