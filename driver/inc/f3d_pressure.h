/* f3d_pressure.c --- 
 * 
 * Filename: f3d_pressure.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: Ching Yuen Ng/ Brian Rak
 * Created: Thu Sep 26 07:05:26 2013
 * Last-Updated: Ching Yuen Ng/ Brian Rak
 *           By: 02-7-2014
 *     Update #: 1
 */

#include <stm32f30x.h>

#define PRESSURE_CS_LOW() GPIO_ResetBits(GPIOA, GPIO_Pin_8);
#define PRESSURE_CS_HIGH() GPIO_SetBits(GPIOA, GPIO_Pin_8);

void f3d_pressure_interface_init();
static uint8_t f3d_pressure_sendbyte(uint8_t byte);
void f3d_pressure_write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
void f3d_pressure_read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);
void f3d_pressure_init(void);
void f3d_pressure_getdata(float* pData, float* tData);
