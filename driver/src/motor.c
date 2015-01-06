//motor.c

#include <stm32f30x_gpio.h>
#include <motor.h>

#define MOTOR_HIGH() GPIO_SetBits(GPIOA, GPIO_Pin_9);
#define MOTOR_LOW() GPIO_ResetBits(GPIOA, GPIO_Pin_9);


void motor_init(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void motor_on(void) {
  MOTOR_HIGH()
 
} 
void motor_off(void) {
  MOTOR_LOW()
} 
