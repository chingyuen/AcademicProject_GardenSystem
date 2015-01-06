#include <f3d_timer2.h>


void f3d_timer2_init(void) {

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;   
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
    TIM_TimeBaseStructure.TIM_Period = 0xFF;          
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0;       
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
    TIM_Cmd(TIM2, ENABLE);
}

void f3d_timer2_setperiod(uint16_t period) {
    static TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0;       
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
    if (period < 0x1F) period = 0x1F;
    period = period << 4;
    TIM_TimeBaseStructure.TIM_Period = period;          
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
}
