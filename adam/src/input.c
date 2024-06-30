/**
 * Weather / input.c
 *
 * Based on @bocianu's code
 *
 * @author Thomas Cherryhomes
 * @email thom dot cherryhomes at gmail dot com
 *
 */

#include <eos.h>
#include <stdbool.h>
#include <smartkeys.h>
#include "input.h"
#include "constants.h"
#include "options.h"
#include "location.h"
#include "state.h"
#include "forecast.h"
#include "screen.h"

extern unsigned short timer;
extern bool forceRefresh;


void input_init(void) { 
    eos_start_read_keyboard(); 
}

bool input_weather(void)
{
    bool key_pressed = false;
    int c;

    c = eos_end_read_keyboard();

    if (c > 1)
    {
        key_pressed = true;
        switch (c)
        {
        case SMARTKEY_I: // F1 OPTIONS
            smartkeys_sound_play(SOUND_CONFIRM);
            if (screen_options(&optData))
                options_save(&optData);
            forceRefresh = true;
            break;

        case SMARTKEY_III: // F3 LOCATION
            smartkeys_sound_play(SOUND_CONFIRM);
            state = LOCATION;
            locData.forceManualLocation = true;
            forceRefresh = true;
            break;

        case SMARTKEY_IV: // F4 FORECAST
            smartkeys_sound_play(SOUND_CONFIRM);
            state = FORECAST;
            forceRefresh = true;
            break;

        case SMARTKEY_V: // F5 UNITS
            smartkeys_sound_play(SOUND_CONFIRM);
            optData.units = (optData.units == IMPERIAL ? METRIC : IMPERIAL);
            options_save(&optData);
            forceRefresh = true;
            break;

        case SMARTKEY_VI: // F6  REFRESH
            smartkeys_sound_play(SOUND_CONFIRM);
            forceRefresh = true;
            break;

        default:
            smartkeys_sound_play(SOUND_NEGATIVE_BUZZ);
            break;
        }
    }

    return key_pressed;
}

bool input_forecast(void)
{
    bool key_pressed = false;
    int c;

    c = eos_end_read_keyboard();

    if (c > 1)
    {
        key_pressed = true;
        switch (c)
        {
        case SMARTKEY_I: // F1 OPTIONS
            smartkeys_sound_play(SOUND_CONFIRM);
            if (screen_options(&optData))
                options_save(&optData);
            forceRefresh = true;
            break;

        case SMARTKEY_II: // F2 NEXT/PREVIOUS PAGE
            smartkeys_sound_play(SOUND_CONFIRM);
            if (forecast_offset == 0)
                forecast_offset = 4;
            else
                forecast_offset = 0;
            forceRefresh = true;
            break;

        case SMARTKEY_III: // F3 LOCATION
            smartkeys_sound_play(SOUND_CONFIRM);
            state = LOCATION;
            locData.forceManualLocation = true;
            forceRefresh = true;
            break;

        case SMARTKEY_IV: // F4 WEATHER
            smartkeys_sound_play(SOUND_CONFIRM);
            state = WEATHER;
            forceRefresh = true;
            break;

        case SMARTKEY_V: // F5 UNITS
            smartkeys_sound_play(SOUND_CONFIRM);
            optData.units = (optData.units == IMPERIAL ? METRIC : IMPERIAL);
            options_save(&optData);
            forceRefresh = true;
            break;

        case SMARTKEY_VI: // F6 REFRESH
            smartkeys_sound_play(SOUND_CONFIRM);
            forceRefresh = true;
            break;

        default:
            smartkeys_sound_play(SOUND_OOPS);
            break;
        }
    }

    return key_pressed;
}
