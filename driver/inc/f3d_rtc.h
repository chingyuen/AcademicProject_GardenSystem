/* f3d_rtc.h --- 
 * 
 * Filename: f3d_rtc.c
 * Description: Header file for real time clock
 * Author: Ching Yuen Ng/ Brian Rak
 * Maintainer: 
 * Created: Fri Feb 21 2014
 * Last-Updated: 02/21/2014
 *           By: brak/ching
 *     Update #: 1
 * Keywords: 
 * Compatibility: 
 * 
 */

#include <stm32f30x.h>
#include <stm32f30x_rtc.h>

volatile extern int standby_flag;

//int LSE_init(void);

void RTC_LSIinit(void);
void RTC_init(void);
int rtc_settime(char *buf);
void f3d_standby_init(void);
