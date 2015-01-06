/* f3d_uart.c --- 
 * 
 * Filename: f3d_uart.c
 * Description: 
 * Author: Bryce Himebaugh
 * Maintainer: Ching Yuen Ng/ Brian Rak
 * Created: Thu Sep 26 07:05:26 2013
 * Last-Updated: Ching Yuen Ng/ Brian Rak
 *           By: 01-24-2014
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
#include <f3d_uart.h>

#define INTERRUPT 1

void f3d_uart_init(void) {


  /* INTERRUPT QUEUES INIT */
#ifdef INTERRUPT
  init_queue(&rxbuf);
  init_queue(&txbuf);
#endif

  // Initialization routines related to UART1

  //GPIOC Clock Enable
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  //UART1 Clock Enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  //TX(Pin4) RX(Pin5) Init
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC,&GPIO_InitStructure);

  //Alternate functions
  GPIO_PinAFConfig(GPIOC,4,GPIO_AF_7);
  GPIO_PinAFConfig(GPIOC,5,GPIO_AF_7);


  //UART1 Configuration
  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1 ,&USART_InitStructure);
  USART_Cmd(USART1 , ENABLE);

  //NVIC Config
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x08;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x08;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


  USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);
}

void flush_uart(void) {
  USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
}

int putchar(int c) {

#ifdef INTERRUPT
  while (!enqueue(&txbuf, c));
#else
  while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) ==
      (uint16_t)RESET); USART_SendData(USART1, c);
#endif
  return 0;
} 

int getchar(void) {
#ifdef INTERRUPT
  int ch;
  while (!(ch=dequeue(&rxbuf)));
  return (ch);
#else
  while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == (uint16_t)RESET)
    return USART_ReceiveData(USART1);
#endif
}

void putstring(char *s) {
  while(*s!= '\0'){
    putchar(*s++);
  }
}

void USART1_IRQHandler(void) {
  int ch;

  if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE)) {
    ch = USART_ReceiveData(USART1);
    while (!enqueue(&rxbuf,ch));
  }
  if (USART_GetFlagStatus(USART1,USART_FLAG_TXE)) {
    ch = dequeue(&txbuf);
    if (ch) {
      USART_SendData(USART1,ch);
    }
    else {
      USART_ITConfig(USART1,USART_IT_TXE,DISABLE); 
    }
  }
}

//new version of getchar(), it will not be waiting for a input so that it won't stop while running.
int readchar(void){
  if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == (uint16_t)SET){
    int c = USART_ReceiveData(USART1);
    return c;
  }
}

/* f3d_uart.c ends here */
