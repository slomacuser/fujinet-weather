#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<time.h>
#include	<conio.h>
#include	"hgrtext.h"
#include	"weatherdefs.h"
#include	"weatherdisp.h"

extern char disp_page;
extern char current_screen;
extern enum unit_option unit_opt;

char c_str[] = {0x22, 0x43, 0x00};
char f_str[] = {0x22, 0x46,0x00};
char min_str[] = {0x23, 0x24,0x00};
char max_str[] = {0x26, 0x27,0x00};
char precip_str[] = {0x28, 0x29, 0x00};

char *temp_unit[] = {c_str, f_str};
char *speed_unit[] = {" m/s", "mph"};

char *wind_deg[] = {" S", " SV", " V", " JV", " J", " JZ", " Z", " SZ"};

char current_forecast_page = 0;

//
// decode description string from weather code
//
void decode_description(char code, char *buf) {
	switch (code) {
		case 0:
			strcpy(buf, "Jasno nebo");
			break;
		case 1:
			strcpy(buf, "Pretezno jasno");
			break;
		case 2:
		 	strcpy(buf, "Delno oblacno");
            break;
		case 3:
		 	strcpy(buf, "Oblacno");
            break;
		case 45:
		 	strcpy(buf, "Megla");
            break;
		case 48:
		 	strcpy(buf, "Megla z ivjem");
            break;
		case 51:
		 	strcpy(buf, "Rahlo rosenje");
            break;
		case 53:
		 	strcpy(buf, "Zmerno rosenje");
            break;
		case 55:
		 	strcpy(buf, "Gosto rosenje");
            break;
		case 56:
		 	strcpy(buf, "Rahlo zmrzovanje rosenja");
            break;
		case 57:
		 	strcpy(buf, "Gosto zmrzovanje rosenja");
            break;
		case 61:
		 	strcpy(buf, "Rahlo dezevje");
            break;
		case 63:
		 	strcpy(buf, "Zmerno dezevje");
            break;
		case 65:
		 	strcpy(buf, "Mocno dezevje");
            break;
		case 66:
		 	strcpy(buf, "Rahlo zmrzovanje dezja");
            break;
		case 67:
		 	strcpy(buf, "Mocno zmrzovanje dezja");
            break;
		case 71:
		 	strcpy(buf, "Rahlo snezenje");
            break;
		case 73:
		 	strcpy(buf, "Zmerno snezenje");
            break;
		case 75:
		 	strcpy(buf, "Mocno snezenje");
            break;
		case 77:
		 	strcpy(buf, "Snezna zrna");
            break;
		case 80:
		 	strcpy(buf, "Rahle plohe");
            break;
		case 81:
		 	strcpy(buf, "Zmerne plohe");
            break;
		case 82:
		 	strcpy(buf, "Mocne plohe");
            break;
		case 85:
		 	strcpy(buf, "Rahle snezne plohe");
            break;
		case 86:
		 	strcpy(buf, "Mocne snezne plohe");
            break;
		case 95:
		case 96:
		case 99:
		 	strcpy(buf, "Nevihta");
            break;
		default:
		 	strcpy(buf, "???");
	}
}
//
// extract time string 'hh:mm'
//
char *time_str(char *buf) {
	*(buf + 16) = 0x00;
	return((buf + 11));
}
//
//
char icon_code(char code) {
	char	result;

	switch (code) {
// clear
		case	0:
			result = 0;
			break;
// mainly clear
		case	1:
			result = 1;
			break;
// partly cloudy
		case	2:
			result = 2;
			break;
// cloud
		case	3:
			result = 3;
			break;
// rain showers
		case	56:
		case	57:
		case	80:
		case	81:
		case	82:
			result = 4;
			break;
// drizzle, rain
		case	51:
		case	53:
		case	55:
		case	61:
		case	63:
		case	65:
		case	66:
		case	67:
			result = 5;
			break;
// thunderstorm
		case	95:
		case	96:
		case	99:
			result = 6;
			break;
// snow
		case	71:
		case	73:
		case	75:
		case	77:
		case	85:
		case	86:
			result = 7;
			break;
// fog
		case	45:
		case	48:
			result = 8;
			break;
		default:
			result = 1;
	}
	return (result);
}

#define HIRES_PAGE1 0x2000
#define HIRES_SIZE  0x2000

void clear_hires_page1() {
    memset((void*)HIRES_PAGE1, 0, HIRES_SIZE);
}


void draw_weather_icon(char code, char col, char row) {
	set_colrow(col, row);
	draw_tile(icon_code(code));
}

// draw sunrise / sunset icon
void draw_sunrs() {
	set_colrow(26, 11);
	draw_tile(9);
}

char padding_center(char *s) {
	char len;

	len = strlen(s);
	return((40 - len) / 2);
}
//
// main display function
//
void disp_weather(WEATHER *wi) {
	char row;
	char time_buf[LINE_LEN];
	char prbuf[LINE_LEN];
	long localtime;
	long visi;
	char wind_idx;

	if (current_screen == 1) {
		return;
	}
	current_screen = 1;
	current_forecast_page = 0;

	init_hgr();
	clear_hires_page1();
//

// line 8 name
	sprintf(prbuf, "%s, %s", wi->name, wi->country); 
	set_colrow(padding_center(prbuf), 8);
	draw_string(prbuf);

//  line 0 datetime
	localtime = wi->td + wi->tz;
    strcpy(prbuf, ctime((const unsigned long *)&localtime));
	prbuf[strlen(prbuf)-1] = 0x00;
	set_colrow(padding_center(prbuf), 0);
	draw_string(prbuf);

//  line 2  temperature
	row = 2;
	sprintf(prbuf, "%s%s", wi->temp, temp_unit[unit_opt]);
	set_colrow(padding_center(prbuf), row);
    draw_string(prbuf);

//  line 4 description
	decode_description(wi->icon, prbuf);
	set_colrow(12,4);
	draw_string(prbuf);

//  line 5 pressure
	set_colrow(12,5);
	sprintf(prbuf, "%s%s", wi->pressure, " hPa");
	draw_string(prbuf);

//  line 6 icon
	draw_weather_icon(wi->icon, 6, 4);

//  line 11,12 sunrise/set icon
	draw_sunrs();

//	line 11 sunrise
	localtime = wi->sunrise + wi->tz;
    strcpy(prbuf, ctime((const unsigned long *)&localtime));
	strcpy(time_buf, time_str(prbuf));
	set_colrow(32,11);
	draw_string(time_buf);

//  line 12 runset
	localtime = wi->sunset + wi->tz;
    strcpy(prbuf, ctime((const unsigned long *)&localtime));
	strcpy(time_buf, time_str(prbuf));
	set_colrow(32,12);
	draw_string(time_buf);


//  line 10-11 Apparent Temperature
	row = 10;
	set_row(row);
	sprintf(prbuf, "Navidezno  : %s%s", wi->feels_like, temp_unit[unit_opt]);
	draw_string(prbuf);
	row = 11;
    set_row(row);
    draw_string(" Temp");

//  line 12 wind speed, deg
	row = 12;
    set_row(row);
	wind_idx = atoi(wi->wind_deg) / 45;
	sprintf(prbuf, "Hitrost vetra:  %s%s%s", wi->wind_speed, speed_unit[unit_opt], wind_deg[wind_idx]);
    draw_string(prbuf);

//  line 14 dew point
	row = 14;
	set_row(row);
	sprintf(prbuf, "Tocka rosisca : %s%s", wi->dew_point, temp_unit[unit_opt]);
	draw_string(prbuf);

//  line 14 humidity
	set_colrow(24, row);
	sprintf(prbuf, "Vlaznost: %s%%", wi->humidity);
	draw_string(prbuf);

//  line 15 visibility
	row = 15;
	set_row(row);
	strcpy(prbuf, wi->visibility);
	visi = atol(prbuf) / 1000;
	sprintf(prbuf, "Vidljivost: %ld km", visi);
	draw_string(prbuf);

//  line 15 clouds
	set_colrow(24, row);
	sprintf(prbuf, "Oblacnost  : %s%%", wi->clouds);
	draw_string(prbuf);

//  line 18 timezone
	row = 18;
	set_colrow(2, row);
	sprintf(prbuf, "Casovni pas: %s", wi->timezone);
	draw_string(prbuf);
}

//
//  display forecast screen
//
void disp_forecast(FORECAST *fc, char p) {
	char	i;
	char	page;
	char    tdbuf[LINE_LEN];
	char	prbuf[QUARTER_LEN];
	long	localtime;
	char	wind_idx;

	if (p == current_forecast_page) {
		return;
	}
	current_screen = 2;
	current_forecast_page = p;
		
	page = (p-1) * 4;
	init_hgr();
	clear_hires_page1();

//	draw heder
	set_colrow(0, 9);
	draw_string(max_str);
	set_colrow(0, 10);
	draw_string(min_str);
	set_colrow(0, 12);
	draw_string("UV ");
	set_colrow(0, 17);
	draw_string(precip_str);

	for (i=0; i<=3; i++) {
//   day         
    	localtime = fc->day[i+page].td;
		strcpy(tdbuf, ctime((const unsigned long *)&localtime));
    	strncpy(prbuf, (char *)&tdbuf[8], 2);
		prbuf[2] = 0x00;
		set_colrow((i*10)+5, 1);
		draw_string(prbuf);
		
//   month
    	strncpy(prbuf, (char *)&tdbuf[4], 3);
		prbuf[3] = 0x00;
		set_colrow((i*10)+5, 2);
		draw_string(prbuf);

//   weekday
    	strncpy(prbuf, (char *)&tdbuf[0], 3);
		set_colrow((i*10)+5, 7);
		draw_string(prbuf);

//   weather icon
		draw_weather_icon(fc->day[i+page].icon, (i*10)+4, 4);
//debug
//		gotoxy((i*10)+4, 22);
//		cprintf("#%2d", fc->day[i+page].icon);

//   max temp
		sprintf(prbuf, "%s%s", fc->day[i+page].temp_max, temp_unit[unit_opt]);
		set_colrow((i*10)+3, 9);
		draw_string(prbuf);

//   min temp
		sprintf(prbuf, "%s%s", fc->day[i+page].temp_min, temp_unit[unit_opt]);
		set_colrow((i*10)+3, 10);
		draw_string(prbuf);

//   wind degree
    	wind_idx = atoi(fc->day[i+page].wind_deg) / 45;
		sprintf(prbuf, "Wind:%s", wind_deg[i+page]);
		set_colrow((i*10)+2, 14);
		draw_string(prbuf);

//   wind speed
		sprintf(prbuf, "%s%s", fc->day[i+page].wind_speed, speed_unit[unit_opt]);
		set_colrow((i*10)+2, 15);
		draw_string(prbuf);

//   uv index max
		sprintf(prbuf, " %s ", fc->day[i+page].uv_index_max);
		set_colrow((i*10)+3, 12);
		draw_string(prbuf);

//   precipitation sum
		sprintf(prbuf, "%s mm", fc->day[i+page].precipitation_sum);
		set_colrow((i*10)+4, 17);
		draw_string(prbuf);

	}
}
