/* f3d_systick.c --- 
 * 
 * Filename: f3d_systick.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: Ching Yuen Ng/ Brian Rak
 * Created: Thu Sep 26 07:05:26 2013
 * Last-Updated: Ching Yuen Ng/ Brian Rak
 *           By: 03-02-2014
 *     Update #: 2
 * Keywords: 
 * Compatibility: 
 * 
 */


/* Copyright (c) 2004-2007 The Trustees of Indiana University and 
 * Indiana University Research and Technology Corporation.  
 * 
 * All rights reserved. 
 * 
 * Additional copyrights may follow 
 */

/* Code: */
#include <f3d_systick.h>
#include <f3d_led.h>
#include <f3d_buzzer.h>
#include <f3d_uart.h>
#define SYSTICK_INT_SEC 100 // .1 second pulse 10Hz

int hold_count=0, hold_count2=0;
/*
 * User button State
 */
extern uint8_t button_check_state, button_check_state2;
extern volatile uint8_t standby_count;
extern volatile uint16_t button_state, button_state2;
extern volatile uint16_t time_passed, time_passed2;
uint32_t delay_count = 0;
extern uint8_t recently_pressed;

void f3d_systick_init(void) {
    // setup systick rate of 100hz.
    SysTick_Config(SystemCoreClock/SYSTICK_INT_SEC);
}

void SysTick_Handler(void) {
    // changes the state machine for user button presses
    delay_count++; // general counter for relative time
    recently_pressed = recently_pressed ? recently_pressed - 1 : 0;
    if (button_check_state) {
        uint8_t cur_state = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
        button_state = cur_state ? button_state + cur_state : 0;
        hold_count = cur_state ? hold_count + cur_state : 0;
        time_passed++;

    }
    if (button_check_state2) {
        uint8_t cur_state2 = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_13) ^ 1;
        button_state2 = cur_state2 ? button_state2 + cur_state2 : 0;
        hold_count2 = cur_state2 ? hold_count2 + cur_state2 : 0;
        time_passed2++;
    }
    // end button press state machine

    if (delay_count % 100 == 0)
        standby_count++;

    if (!queue_empty(&txbuf)) {
        flush_uart();
    }
}

/* f3d_systick.c ends here */
