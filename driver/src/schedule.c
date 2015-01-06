/* schedule.c --- 
 * 
 * Filename: schedule.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Fri Feb  8 04:46:37 2013 (-0500)
 * Version: 
 * Last-Updated: 
 *           By: 
 *     Update #: 0
 * URL: 
 * Doc URL: 
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change Log:
 * 
 * 
 */

/* This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth
 * Floor, Boston, MA 02110-1301, USA.
 */

/* Code: */

#include <stm32f30x.h>
#include <schedule.h>
#include <stdio.h>
#include <f3d_rtc.h>

void schedule(void) {
  static int counter_100mS = 0; 
  static int counter_1S = 0;
  RTC_DateTypeDef RTC_DateStructure;
  RTC_TimeTypeDef RTC_TimeStructure;
 
  if (counter_100mS++==10) {
    counter_100mS=0;
  }
  if (counter_1S++==100) {
    counter_1S=0;
    //printf("0x%08x\n",get_fattime());

    /* RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);  */
    /* RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);  */
    /* printf("%0.2d/%0.2d/20%0.2d,",RTC_DateStructure.RTC_Month, RTC_DateStructure.RTC_Date, RTC_DateStructure.RTC_Year);  */
    /* printf("%0.2d:%0.2d:%0.2d\n",RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds); */
  }
}

/* schedule.c ends here */
