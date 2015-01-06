/* f3d_button.c --- 
 * 
 * Filename: f3d_button.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: Ching Yuen Ng/ Brian Rak
 * Created: Thu Jan 17 16:21:57 2013 (-0500)
 * Version: 
 * Last-Updated: 01/17/2014
 *           By: Ching Yuen Ng
 *     Update #: 1
 * URL: 
 * Doc URL: 
 * Keywords
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
#include <stm32f30x_gpio.h>
#include <f3d_button.h>

uint8_t button_check_state = 0, button_check_state2 = 0;
volatile uint16_t button_state = 0, button_state2 = 0;
volatile uint16_t time_passed = 0, time_passed2 = 0;
uint8_t recently_pressed = 0;

void f3d_button_init() {
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Configure PD13 as an Inputs, extra button
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

 
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_2);
}

/* DEPENDENT ON SYSTICK HANDLER TO UPDATE PRESSES
 * Accounts for noise during a button press
 * when the counter gets to a certain value,
 * the button is pressed
 * Count is abstracted so that function can be used for
 * any number of buttons
 * If we are logging, the system is checking the button state every x ms
 * Next if the count is at a certain BUTTON_SENSITIVITY, then the button is pressed
 * Finally, we can turn off logging after a certain threshold
 */
uint8_t f3d_button_state_read(uint8_t *btn_check_state,
                             volatile uint16_t *btn_state, // counter
                             volatile uint16_t *tim_passed)
{
    uint8_t return_value = 0;
    /* First ask are we in the button check state? */
    if (*btn_check_state) {
        /* Check for Timeout if we we're logging */
        if (*tim_passed > BUTTON_TIMEOUT) {
            // turn off the log, reset counters
            *btn_check_state = 0;
            *btn_state = 0;
            *tim_passed = 0;
            // return_value = 0, since we have exceeded the timer
        } else { // we have not timed out yet, so check our counter
            if (*btn_state > BUTTON_SENSITIVITY && !recently_pressed) {
                recently_pressed = BUTTON_DELAY;
                return_value = 1;
                *tim_passed = 0; // reset counters to maintain our button state of on
            } else {}
        }
    } else {
        // here we are not in the logging state currently, so we need to toggle it on
        // since the system asked to check for a button press
        // reset counters just in case data is left over from last button press
        *btn_state = 0;
        *tim_passed = 0;
        *btn_check_state = 1; // finally turn on the checking state
    }

    return return_value;
}

//return the button state, either 1 or 0
uint8_t f3d_button_read() {
  //uint8_t btn_state = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
  return f3d_button_state_read(&button_check_state, &button_state, &time_passed);
}

uint8_t f3d_extra_button(){
  //uint8_t btn_state = GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_13) ^ 1;
  return f3d_button_state_read(&button_check_state2, &button_state2, &time_passed2);
}
/* f3d_button.c ends here */
