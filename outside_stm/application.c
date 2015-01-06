/**
 * This file provides the interface for the outside stm watering garden.
 */

#include "application.h"

#define MAX_RX_TIMEOUT 10000
#define MAX_TX_TIMEOUT 10000

RTC_DateTypeDef   RTC_CurrentDate;
RTC_TimeTypeDef   RTC_CurrentTime;
uint8_t _pump_state = 0;

// sends a packet over the network using the nordics
// converts data to send it over the network if it is not of type char *
void app_send_nordic_packet(char *txdata, char *rxdata) {

    int index;
    int rx_timeout = 0, tx_timeout = 0;

    printf("\n");
    printf("Node: transmit character %c\n",txdata);
    nrf24l01base_write_tx_payload(txdata, 32, true);      // nordic writes a character
    while(!(nrf24l01base_irq_pin_active() && (nrf24l01base_irq_tx_ds_active() || nrf24l01base_irq_max_rt_active()))) {
        // error catching in case we get stuck
        if (tx_timeout++ > MAX_TX_TIMEOUT) {
            printf("tx timeout\n");
            break;
        }
    }// wait until it is gone

    //Node Listen Mode if the maximum retry limit was not hit
    if (!nrf24l01base_irq_max_rt_active()) {
        nrf24l01base_irq_clear_all();
        f3d_delay_uS(130);
        nrf24l01base_set_as_rx(true);
        printf("true\n");

        printf("node reception\n");
        while(!(nrf24l01base_irq_pin_active() && nrf24l01base_irq_rx_dr_active())) {
            if (rx_timeout++ > MAX_RX_TIMEOUT) {
                printf("rx timeout\n");
                break;
            }
        }
        if (rx_timeout < MAX_RX_TIMEOUT) {
            nrf24l01base_read_rx_payload(rxdata, 32);

            printf("Node Data: ");
            for (index=0;index<32;index++) {
                printf("%c",rxdata[index]);
            }
            printf("\n");

            printf("Node: Receive character %c\n",rxdata);
        }
        nrf24l01base_irq_clear_all();
        f3d_delay_uS(130);
        nrf24l01base_set_as_tx();

    }
    else {
        nrf24l01base_flush_tx(); //get the unsent character out of the TX FIFO
        nrf24l01base_irq_clear_all(); //clear all interrupts
        printf("Node: Failed to send %c\n",txdata);
    }

}

/* Fits data from the moisture sensor, the pump status, and 
 * the solar battery charge level, and the current date/time, 
 * shifts and stores them to fit the API of the nordic packet 
 * to send over the network
 */
void app_prepare_nordic_packet (int *moisture_data,
        int *power_level_data,
        RTC_DateTypeDef *RTC_CurrentDate,
        RTC_TimeTypeDef *RTC_CurrentTime,
        char *packet) {

    // split the moisture data into bytes
    packet[3]= *moisture_data;
    *moisture_data= (*moisture_data)>>8;
    packet[2]= *moisture_data;
    *moisture_data= (*moisture_data)>>8;
    packet[1]= *moisture_data;
    *moisture_data= (*moisture_data)>>8;
    packet[0]= *moisture_data;

    // pump status data tells us if the pump is on or off
    packet[4] = _pump_state;

    // split the power level data into bytes
    packet[8]= *power_level_data;
    *power_level_data= (*power_level_data)>>8;
    packet[7]= *power_level_data;
    *power_level_data= (*power_level_data)>>8;
    packet[6]= *power_level_data;
    *power_level_data= (*power_level_data)>>8;
    packet[5]= *power_level_data;

    // split the current date
    packet[9] = RTC_CurrentDate->RTC_Month;
    packet[10] = RTC_CurrentDate->RTC_Date;
    // remember to add at least 2000 to the current year
    packet[11] = RTC_CurrentDate->RTC_Year;     

    // split the current time
    packet[12] = RTC_CurrentTime->RTC_Hours;
    packet[13] = RTC_CurrentTime->RTC_Minutes;
    packet[14] = RTC_CurrentTime->RTC_Seconds;
}

// initializes all of the drivers for the peripherals
void app_init(void) {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    f3d_delay_init();
    f3d_delay_uS(10);
    f3d_led_init();
    f3d_delay_uS(10);
    f3d_uart_init();
    f3d_delay_uS(10);
    f3d_systick_init();
    f3d_delay_uS(10);
    f3d_i2c1_init();
    f3d_delay_uS(10);
    f3d_timer2_init();
    f3d_delay_uS(10);
    f3d_dac_init();
    f3d_delay_uS(10);
    f3d_a2d_init();
    f3d_delay_uS(10);
    ds_wifibase_init();
    f3d_delay_uS(10);
    f3d_standby_init();
    //RTC_init();
    //RTC_LSI_init();
    motor_init();

    /* nrf24l01base_initialize_debug(false, 1, false); // Setup Node at transmitter, Standard Shockburst */
    nrf24l01base_initialize_debug(false, 32, true); // Enhanced Shockburst, Auto Ack turned on
    nrf24l01base_clear_flush();
}

// reads and stores data into a buffer until full
void app_read_moisture_data(int *data) {
    *data = f3d_read_adc(MOISTURE);
}

// turns on the pump
// has no effect is the pump is running
void app_turn_on_pump(void) {
    _pump_state = 1;
    motor_on();
}

// turns off the pump
// has no effect is the pump is not running
void app_turn_off_pump(void) {
    _pump_state = 0;
    motor_off();
}

// check to see if the pump is turned on
// @return 0 if the pump is NOT on, 1 if it is on
int app_check_pump_status(void) {
    return _pump_state;
}

// reads the power level of the battery
// stores the data into the pointer
void app_read_battery_power(int *data) {
    *data = f3d_read_adc(BATTERY);
}

void app_enter_standby(void) {
    //f3d_led_all_on();
    RTC_GetTime(RTC_Format_BIN,&RTC_CurrentTime);

    printf("Wakeup Counter: %d\n", RTC_GetWakeUpCounter());
    printf("WUT_IT_Status : %d\n",RTC_GetITStatus(RTC_IT_WUT));
    printf("%d:%d:%d\n", RTC_CurrentTime.RTC_Hours,
            RTC_CurrentTime.RTC_Minutes, RTC_CurrentTime.RTC_Seconds);
    printf("Month: %d Year: %d\n", RTC_CurrentDate.RTC_Month,
            RTC_CurrentDate.RTC_Year); 

    delay(1000);    
    //f3d_led_all_off();
    //delay(200);

    // Enable Wakeup Counter
    //RTC_WakeUpCmd(ENABLE);

    //f3d_led_all_off();
    // Enter Stop Mode
    //PWR_EnterSTOPMode(PWR_SLEEPEntry_WFI); 
    PWR_EnterSTANDBYMode(); 
    //app_init2(); 
    //f3d_led_all_on();
}

void app_print_nordic_data(char *data) {
    int index;
    for (index=0;index<32;index+=4) {
        data[index] = 'a'+index;
    }
    printf("Node Data: ");
    for (index=0;index<32;index++) {
        printf("%c",data[index]);
    }
}

void app_init2(void) {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    f3d_delay_init();
    f3d_delay_uS(10);
    f3d_led_init();
    f3d_delay_uS(10);
    f3d_uart_init();
    f3d_delay_uS(10);
    f3d_systick_init();
    f3d_delay_uS(10);
    f3d_i2c1_init();
    f3d_delay_uS(10);
    f3d_timer2_init();
    f3d_delay_uS(10);
    f3d_dac_init();
    f3d_delay_uS(10);
    f3d_a2d_init();
    f3d_delay_uS(10);
    ds_wifibase_init();
    f3d_delay_uS(10);
    //RTC_init();
    //RTC_LSI_init();
    motor_init();

    /* nrf24l01base_initialize_debug(false, 1, false); // Setup Node at transmitter, Standard Shockburst */
    nrf24l01base_initialize_debug(false, 32, true); // Enhanced Shockburst, Auto Ack turned on
    nrf24l01base_clear_flush();
}

