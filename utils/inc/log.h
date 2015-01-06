/* log.h --- 
 * 
 * Filename: log.h
 * Description: logging utilties for stm32f30x periphs
 * Author: 
 * Maintainer: Ching Yuen Ng/ Brian Rak
 * Created: Wed 2-11-14
 * Last-Updated: 02-11-14
 *           By: Ching Yuen Ng/ Brian Rak
 *     Update #: 1
 * Keywords: 
 * Compatibility: 
 * 
 */

#include <stm32f30x.h>
#include <f3d_gyro.h>
#include <f3d_pressure.h>
#include <f3d_sdcard.h>
#include <diskio.h>
#include <pressure_utils.h>

#include <stdio.h>

#define LOG_SIZE 120
#define GYRO_DATA 1
#define PRESSURE_DATA 2

/* define statments for the log event table */
#define ERROR 0
#define BUTTON_PRESS 1
#define BUTTON2_PRESS 2
#define GYRO_MOVE 3
#define START_SET_TIME 4
#define END_SET_TIME 5
#define SOUND_ALARM 6
#define QUIET_ALARM 7

/* the buffer size that, when full, is able to be written to the 512 byte block
 * on the SD card
 */
extern const char *log_buffer[LOG_SIZE][50];
extern const char *event[10];
extern float pressure, temp, altitude, gyro_buffer[2];
extern uint16_t log_index;

//float altitude_cache[LOG_SIZE / 10];
//float gyro_cache[LOG_SIZE][2]; // REMEMBER TO CHANGE THIS IF SYSTIC CHNGS
// also need to change in log_data in cfile
void log_data(const char *event, const char *date_time);
void get_data(int i);
int dump_log_to_disk(FIL *fil, UINT bw);
void get_timestamp(
        int current_hour,
        int current_min,
        int current_seconds,
        int current_month,
        int current_day,
        int current_year,
        char *buffer);
