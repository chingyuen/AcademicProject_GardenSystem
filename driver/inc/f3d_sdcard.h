/* f3d_sdcard.h --- 
 * 
 * Filename: f3d_sdcard.h
 * Description: SD card header file
 * Author: Brian Rak, Ching Ng
 * Maintainer: 
 * Created: Fri Feb 28 2014
 * Last-Updated: 
 *           By: 
 *     Update #: 0
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
#include <stm32f30x.h>
#include <ff.h>

/* enum spiSpeed { SPI_SLOW , SPI_MEDIUM, SPI_FAST }; */

#define SPI_SLOW (uint16_t) SPI_BaudRatePrescaler_64
#define SPI_MEDIUM (uint16_t) SPI_BaudRatePrescaler_8
#define SPI_FAST (uint16_t) SPI_BaudRatePrescaler_2

#define SD_CS_HIGH() GPIO_SetBits(GPIOB, GPIO_Pin_8)
#define SD_CS_LOW()  GPIO_ResetBits(GPIOB, GPIO_Pin_8)

void f3d_sdcard_init(void);
int spiReadWrite(SPI_TypeDef *SPIx, uint8_t *rbuf, 
		 const uint8_t *tbuf, int cnt, uint16_t speed);
int spiReadWrite16(SPI_TypeDef *SPIx, uint16_t *rbuf, 
		   const uint16_t *tbuf, int cnt, uint16_t speed);
void f3d_sdcard_readwrite(uint8_t *, uint8_t *, int);
uint32_t get_fattime(void);
void die (FRESULT);
static void xmit_mmc(const BYTE* buff, UINT bc);
static void rcvr_mmc (BYTE *buff, UINT bc);

/* f3d_sdcard.h ends here */

