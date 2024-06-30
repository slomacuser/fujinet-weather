/**
 * Weather
 *
 * Based on @bocianu's code
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */

#ifndef INPUT_H
#define INPUT_H
#include <stdbool.h>

/*
 input_input
 Prepares the adam for keyboard reading
*/
void input_init(void);
/*
 input_weather
 Returns true is a valid smartkey has been pressed
 Sets forceRefresh if screen needs updating
 NOTE: you must call input_init if this returns true
*/
bool input_weather(void);

/*
 input_forecast
 Returns true is a valid smartkey has been pressed
 Sets forceRefresh if screen needs updating
 NOTE: you must call input_init if this returns true
*/

bool input_forecast(void);

#endif /* INPUT_H */
