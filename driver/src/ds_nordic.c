/* ds_nordic.c --- 
 * 
 * Filename: ds_nordic.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: 
 * Created: Thu Feb 21 11:15:36 2013
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
#include <ds_nordic.h>

void ds_nordic_init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  // Initialization for SCK, MISO, MOSI
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_6);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_6);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_6);

  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);


  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(SPI3);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(SPI3, &SPI_InitStructure);
  /* Configure the RX FIFO Threshold */
  SPI_RxFIFOThresholdConfig(SPI3, SPI_RxFIFOThreshold_QF);
  /* Enable SPI3  */
  SPI_Cmd(SPI3, ENABLE);

  /* Configure Outputs for CS (PC1) and CE (PC2) */
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* Initial Write to the CS and CE   */
  RF_CS_HIGH();
  RF_CE_HIGH();
  
  /* Configure Inputs for IRQ (PC3) */
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // Command Initialization
  ds_nordic_command_initialization();
} 

void ds_nordic_readwrite(uint8_t *rxbuf, uint8_t *txbuf, int count) {
  int index;

  for (index=0;index<count;index++) {
    if (txbuf) {
      SPI_SendData8(SPI3, *txbuf++);
    }
    else {
      SPI_SendData8(SPI3, 0xFF);
    }
    while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_RXNE) == RESET);
    if (rxbuf) {
      *rxbuf++ = (uint8_t) SPI_ReceiveData8(SPI3);
    }
    else {
      SPI_ReceiveData8(SPI3);      
    }
  }
}


uint8_t ds_nordic_send_spi_byte(uint8_t data) {
  SPI_SendData8(SPI3,data);
  while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_RXNE) == RESET);
  return SPI_ReceiveData8(SPI3);      
}

uint8_t ds_nordic_send_command(uint8_t command, uint8_t data) {
  uint8_t status;
  RF_CE_LOW();
  RF_CS_LOW();
  ds_nordic_send_spi_byte(command);
  status = ds_nordic_send_spi_byte(data);
  RF_CS_HIGH();
  return(status);
}

uint8_t ds_nordic_receive_data(void) {
  // volatile int i;
  uint8_t retval;
  RF_CS_LOW();
  ds_nordic_send_spi_byte(0x61);
  retval = ds_nordic_send_spi_byte(0xFF);
  ds_nordic_send_spi_byte(0xFF);
  ds_nordic_send_spi_byte(0xFF);
  ds_nordic_send_spi_byte(0xFF);
  RF_CS_HIGH();
  f3d_delay_uS(500);
 // for (i=0;i<1000;i++);
  RF_CS_LOW();
  ds_nordic_send_spi_byte(0xE2);
  RF_CS_HIGH();
  ds_nordic_send_command(0x27,0x40);
  RF_CE_HIGH();
  return (retval);
}


void ds_nordic_command_initialization(void) {
  RF_CE_LOW();
  ds_nordic_send_command(0x20,0x39);
  ds_nordic_send_command(0x21,0x00);
  ds_nordic_send_command(0x23,0x03);
  ds_nordic_send_command(0x26,0x07);
  ds_nordic_send_command(0x31,0x04);
  ds_nordic_send_command(0x25,0x02);

  RF_CS_LOW();
  ds_nordic_send_spi_byte(0x2A);
  ds_nordic_send_spi_byte(0xE7);
  ds_nordic_send_spi_byte(0xE7);
  ds_nordic_send_spi_byte(0xE7);
  ds_nordic_send_spi_byte(0xE7);
  RF_CS_HIGH();

  ds_nordic_send_command(0x20,0x3B);

  RF_CE_HIGH();
}

uint8_t ds_nordic_readirq(void) {
  return (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3));
}



/* ds_nordic.c ends here */
