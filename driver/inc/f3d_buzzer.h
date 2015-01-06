/* f3d_buzzer.h --- 
 * 
 * Filename: f3d_buzzer.h
 * Description: Header for a simple buzzer on Pin D12
 * Author: Brian Rak, Ching Ng
 * Maintainer: 
 * Created: Feb 21 2014
 * Last-Updated: 02/21/2014
 *           By: brak, ching
 *     Update #: 1
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Code: */
#include <stm32f30x.h>

static void f3d_timer4_pins(void);
void f3d_timer4_init(void);
void f3d_frequency(uint32_t freq);
void f3d_hightime_oc1_uS(uint32_t time_uS);
void TIM4_IRQHandler(void);
void buzzer_switch(int buzzer);

/* buzzer.h ends here */
