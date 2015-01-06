/* log.c --- 
 * 
 * Filename: log.c
 * Description: logging utilties for stm32f30x periphs
 * Author: 
 * Maintainer: Ching Yuen Ng/ Brian Rak
 * Created: Wed 2-11-14
 * Last-Updated: 03-02-14
 *           By: Ching Yuen Ng/ Brian Rak
 *     Update #: 1
 * Keywords: 
 * Compatibility: 
 * 
 */


#include <log.h>
#include <string.h>

const char *log_buffer[LOG_SIZE][50];

const char *event[] = {
    "ERROR",
    "BUTTON_PRESS",
    "BUTTON2_PRESS",
    "GYRO_MOVE",
    "START_SET_TIME",
    "END_SET_TIME",
    "START_SET_ALARM",
    "END_SET_ALARM",
    "SOUND_ALARM",
    "QUIET_ALARM"
};

float pressure, temp, altitude, gyro_buffer[2];


/*
 * get_data takes an int to check what kind of data needs to
 * be put into the buffer
 * Gyro = 1
 * Pressure & temp = 2
 * ALL = 3
 */

uint16_t log_index = 0;

void get_data(int i) {
    if (i & 1) { // store gyro
        f3d_gyro_getdata(&gyro_buffer[0]);
    }
    if ((i >> 1) & 1) { // store pressure and temp, convert to altitude
        // uint8_t ctrl2=0x01;
        // f3d_pressure_write(&ctrl2, 0x21, 1);
        f3d_pressure_getdata(&pressure, &temp);
        convert_to_altitude_ft(&pressure, &altitude);
    }
}

void log_data(const char *event, const char *date_time) {
    if (log_index >= LOG_SIZE) {
        log_index = 0;
        //strcpy(&log[log_index++], "LOG BUFFER OVERRUN"); TODO figure out how to get this string into buffer
    }

    // append the strings and store into the buffer
    char *buffer = (char *) log_buffer[log_index++];
    strcpy(buffer, event);
    strcat(buffer, " ");
    strcat(buffer, date_time);
    printf("%s logged\n", buffer);
}

/* Dumps the current log to the SD card
 * check to make sure f_mount is called in main before calling this function!
 */
int dump_log_to_disk(FIL *fil,UINT bw) {
    FRESULT rc;
    printf("Dumping Log to Disk NOW\n");
    printf("Open the file\n");

    rc = f_open(fil, "LOG.TXT", FA_OPEN_EXISTING | FA_WRITE);
    if (rc == FR_NO_FILE) {
      printf("No log found.  Creating \"log.txt\".\n");
      rc = f_open(fil, "LOG.TXT", FA_CREATE_ALWAYS | FA_WRITE);
    }
    printf("done\n");
    if (rc) { die(rc); return bw; } // couldn't open the file
    int i;
    char *str;
    printf("creating str\n");
    for (i = 0; i < log_index; i++) {
      str = (char *) log_buffer[i];
      rc = f_lseek(fil, f_size(fil));
      rc = f_write(fil, str, strlen(str), &bw); // + 1 to write the null terminator
      if (rc) {
	die(rc);
	break;
      }
    }
      rc = f_close(fil);
      if (rc) { die(rc); return 1;} // couldn't write or close the file

      printf("Log has been successfully dumped\n");
      return bw; // success
      

}

void get_timestamp(
        int current_hour,
        int current_min,
        int current_seconds,
        int current_day,
        int current_month,
        int current_year,
        char *buffer)
{
    char hours[3], mins[3], secs[3], day[3], month[3], yr[5];
    sprintf(hours, "%d", current_hour);
    sprintf(mins,  "%d", current_min);
    sprintf(secs,  "%d", current_seconds);
    sprintf(month, "%d", current_month);
    sprintf(day,   "%d", current_day);
    sprintf(yr,    "%d",  current_year);

    printf("TIMESTAMP: %d:%d:%d %d/%d/%d\n", current_hour, current_min, current_seconds,
            current_month, current_day, current_year);
    // copy & concat the strings
    strcpy(buffer, hours);
    strcat(buffer, ":");
    strcat(buffer, mins);
    strcat(buffer, ":");
    strcat(buffer, secs);
    strcat(buffer, ",");
    strcat(buffer, month);
    strcat(buffer, "/");
    strcat(buffer, day);
    strcat(buffer, "/");
    strcat(buffer, yr);
    strcat(buffer, "\n");
}

void log_gyro(char *in) {
  char *buffer = (char *) log_buffer[log_index++];
  strcpy(buffer, in);
  printf("%s logged\n", buffer);
}
