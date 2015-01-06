/**
  * This file provides the interface for the outside stm watering garden.
  */

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers
#include <f3d_led.h>
#include <f3d_button.h>
#include <f3d_delay.h>
#include <f3d_systick.h>
#include <f3d_uart.h>
#include <f3d_rtc.h>
#include <f3d_a2d.h>
#include <f3d_timer2.h>
#include <schedule.h>
#include <ds_wifibase.h>
#include <nrf24l01base.h>
#include <motor.h>
#include <stdio.h>

/* sends a packet over the network using the nordics
 */
void app_send_nordic_packet(char *txdata, char *rxdata);

/* converts an int to char data to send over the network
 */
void app_prepare_nordic_packet(int *moisture_data,
                               int *power_level_data,
                               RTC_DateTypeDef *RTC_CurrentDate,
                               RTC_TimeTypeDef *RTC_CurrentTime,
                               char *packet);

/* Initializes all of the drivers for the peripherals
 */
void app_init(void);

/* reads and stores data into a buffer until full
 */
void app_read_moisture_data(int *data);

/* enters STOP mode to conserve power
 * wakes up using the EXTI hardware interrupt
 */
void app_enter_standby_mode(void);

/* turns on the pump
 * has no effect is the pump is running
 */
void app_turn_on_pump(void);

/* turns off the pump
 * has no effect is the pump is not running
 */
void app_turn_off_pump(void);

/* check to see if the pump is turned on
 * @return 0 if the pump is NOT on, 1 if it is on
 */
int app_check_pump_status(void);

/* reads the power level of the battery
 * stores the data into the pointer
 */
void app_read_battery_power(int *data);

/* enters standby mode by shutting down regulator and
 * releasing all contents in memory
 * board starts up from the beginning when the rising edge
 * of the WKUP pin is detected
 */
void app_enter_standby(void);
void app_print_nordic_data(char *data);

#endif /* _APPLICATION_H_ */
