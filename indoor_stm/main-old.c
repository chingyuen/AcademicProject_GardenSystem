/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: Ching Yuen Ng/ Brain Rak
 * Created: 
 * Last-Updated: 
 *           By: 
 *     Update #
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
/* Code: */

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>
#include <f3d_lcd_sd.h>
#include <f3d_delay.h>
#include <f3d_systick.h>
#include <f3d_uart.h>
#include <f3d_i2c.h>
#include <schedule.h>
#include <ds_nordic.h>
#include <nrf24l01.h>
#include <stdio.h>

/*
//draw circle funtion 
void draw_circle(int radius, int ox, int oy, uint16_t color){
int x,y;
for (x=-radius; x<radius; x++){
int height=(int)sqrt(radius*radius-x*x);
for(y=-height;y<height;y++){
f3d_lcd_drawPixel(x+ox,y+oy,color);
}
}
}

*/

int main() { 
    f3d_delay_init();
    f3d_delay_uS(10);
    f3d_uart_init();
    f3d_delay_uS(10);
    f3d_led_init();
    f3d_delay_uS(10);
    f3d_lcd_init();
    f3d_delay_uS(10);
    f3d_systick_init();
    f3d_delay_uS(10);
    f3d_i2c1_init();
    f3d_delay_uS(10);
    ds_nordic_init();
    f3d_delay_uS(10);

    /*

       f3d_lcd_fillScreen(WHITE);
       f3d_lcd_drawChar(20,80,'X',RED,WHITE);
       f3d_lcd_drawChar(60,80,'Y',GREEN,WHITE);
       f3d_lcd_drawChar(100,80,'Z',BLUE,WHITE);
       f3d_lcd_drawString(50,10,"GYRO",BLACK,WHITE);

       int k=0;
       */

    /* nrf24l01_initialize_debug(false, 1, false);  // Setup Node at transmitter, Standard Shockburst */
    nrf24l01_initialize_debug(false, 32, true);     // Enhanced Shockburst, Auto Ack turned on
    nrf24l01_clear_flush();

    char data[32];
    char index;

    for (index=0;index<32;index++) {
        data[index] = 'a'+index;
    }
    printf("Node Data: ");
    for (index=0;index<32;index++) {
        printf("%c",data[index]);
    }
    printf("\n");
    printf("Node: transmit character %c\n",data);
    nrf24l01_write_tx_payload(data, 32, true);      // nordic writes a character
    while(!(nrf24l01_irq_pin_active() && (nrf24l01_irq_tx_ds_active() || nrf24l01_irq_max_rt_active()))); // wait until it is gone

    // Node Listen Mode if the maximum retry limit was not hit
    if (!nrf24l01_irq_max_rt_active()) {
        nrf24l01_irq_clear_all();
        nrf24l01_set_as_rx(true);
        printf("true\n");
    }
    else {
        nrf24l01_flush_tx(); //get the unsent character out of the TX FIFO
        nrf24l01_irq_clear_all(); //clear all interrupts
        printf("Node: Failed to send %c\n",data);
    }

    // Node Reception
    printf("node reception\n");
    char noderx[32];
    while(!(nrf24l01_irq_pin_active() && nrf24l01_irq_rx_dr_active()));
    nrf24l01_read_rx_payload(noderx, 32);

    printf("Node Data: ");
    for (index=0;index<32;index++) {
        printf("%c",noderx[index]);
    }
    printf("\n");

    printf("Node: Receive character %c\n",noderx);
    nrf24l01_irq_clear_all();
    f3d_delay_uS(130);
    nrf24l01_set_as_tx();

    printf("#### End Wifi Test ####\n");



    while(1){
        f3d_led_all_on();
        printf("a\n");
        f3d_lcd_fillScreen(BLUE);
        f3d_lcd_fillScreen(RED);
        f3d_lcd_fillScreen(GREEN);
    }

}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
    /* Infinite loop */
    /* Use GDB to find out why we're here */
    while (1);
}
#endif


/* main.c ends here */
