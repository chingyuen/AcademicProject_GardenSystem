/* f3d_uart.h --- 
 * 
 * Filename: f3d_uart.h
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: Ching Yuen Ng/ Brian Rak
 * Created: Thu Sep 26 07:05:43 2013
 * Last-Updated: 01-24-2014
 *           By: Ching Yuen Ng/ Brain Rak
 *     Update #: 1
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
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
#include <stm32f30x.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_usart.h>
#include <queue.h>

queue_t rxbuf;
queue_t txbuf;

void f3d_uart_init(void);
int putchar(int);
int getchar(void);
void putstring(char *);
//non-stop version of getchar()
int readchar(void);
void flush_uart(void);

/* f3d_uart.h ends here */
