/* f3d_button.h --- 
 * 
 * Filename: f3d_button.h
 * Description: 
 * Author: Brian Rak
 * Maintainer: brak
 * Created: 03/01/2014
 * Version: 
 * Last-Updated: 03/01/2014
 *           By: 
 *     Update #: 0
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

#define BUTTON_SENSITIVITY 14
#define BUTTON_TIMEOUT 20
#define BUTTON_DELAY 20

extern uint8_t button_check_state, button_check_state2;
extern volatile uint16_t button_state, button_state2;
extern volatile uint16_t time_passed, time_passed2;

void f3d_button_init();
uint8_t f3d_button_read();
uint8_t f3d_extra_button();
uint8_t f3d_button_state_read(
        uint8_t *btn_check_state,
        volatile uint16_t *button_state, // counter
        volatile uint16_t *time_passed);
/* f3d_button.h ends here */
