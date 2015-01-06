/* ds_nordic.h --- 
 * 
 * Filename: ds_nordic.h
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Feb 21 11:24:59 2013
 * Last-Updated: 
 *           By: 
 *     Update #: 0
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

#define RF_CS_LOW()   GPIO_ResetBits(GPIOC, GPIO_Pin_1);
#define RF_CS_HIGH()  GPIO_SetBits(GPIOC, GPIO_Pin_1);
#define RF_CE_LOW()   GPIO_ResetBits(GPIOC, GPIO_Pin_2);
#define RF_CE_HIGH()  GPIO_SetBits(GPIOC, GPIO_Pin_2);

void ds_nordic_init(void);
void ds_nordic_readwrite(uint8_t *,uint8_t *,int);
uint8_t ds_nordic_send_spi_byte(uint8_t);
uint8_t ds_nordic_send_command(uint8_t, uint8_t);
uint8_t ds_nordic_receive_data(void);
void ds_nordic_command_initialization(void);
uint8_t ds_nordic_readirq(void); 



/* ds_nordic.h ends here */
