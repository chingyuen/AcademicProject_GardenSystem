/* alarm_clock_utils.h ---
 *
 * Filename: alarm_clock_utils.h
 * Description:
 * Author: Ching Yuen Ng/ Brian Rak
 * Maintainer:
 * Created: Sat Mar 1 2014
 * Last-Updated: 03/01/2014
 *           By: brak/ching
 *     Update #: 1
 * Keywords:
 * Compatibility:
 *
 */

#include <stm32f30x.h>
#include <f3d_led.h>
#include <f3d_rtc.h>

#include <log.h>
#include <stdio.h>

/* A time struct that holds the current time
 * Created for easy and modular time updates via the
 * read_time function
 */
typedef struct {
    uint8_t current_hour;
    uint8_t current_minute;
    uint8_t current_second;
    uint8_t current_month;
    uint8_t current_day;
    uint32_t current_year;
} TimeStruct;

// minute functions
void f3d_led_min_display(int mins);
void min_right(int n);

// hour functions
void f3d_led_hr_display(int hours);
void hour_right(int n);

// alarm functions
void set_alarm(int *set_unit, int *set_hr, int *set_min);

// other functions
void read_time(TimeStruct *time);
