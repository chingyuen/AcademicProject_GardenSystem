/* ds_wifibase.h --- 
 * 
 * Filename: ds_wifibase.h
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

#define WIFI_CS_LOW()   GPIO_ResetBits(GPIOC, GPIO_Pin_0);
#define WIFI_CS_HIGH()  GPIO_SetBits(GPIOC, GPIO_Pin_0);
#define WIFI_CE_LOW()   GPIO_ResetBits(GPIOC, GPIO_Pin_6);
#define WIFI_CE_HIGH()  GPIO_SetBits(GPIOC, GPIO_Pin_6);

void ds_wifibase_init(void);
void ds_wifibase_readwrite(uint8_t *,uint8_t *,int);
uint8_t ds_wifibase_send_spi_byte(uint8_t);
uint8_t ds_wifibase_send_command(uint8_t, uint8_t);
uint8_t ds_wifibase_receive_data(void);
void ds_wifibase_command_initialization(void);
uint8_t ds_wifibase_readirq(void); 



/* ds_wifibase.h ends here */
