#include "application.h"
#include <stdlib.h>

#define SEND_INTERVAL_SECONDS 60
#define MOISTURE_THRESHOLD    400

// checks if standby flag has been triggered
volatile extern int standby_flag;

// timer for entering standby
volatile uint8_t standby_count;

int main(){

    app_init();

    char txdata[32] = {}, rxdata[32] = {};
    char index;
    int moisture, battery, pump_status;
    RTC_DateTypeDef CurrentDate;
    RTC_TimeTypeDef CurrentTime;
    uint8_t throwaway_count = 0;

    app_read_moisture_data(&moisture);

    if (moisture < MOISTURE_THRESHOLD) {
        app_turn_on_pump();
        printf("pump turned on \n");
    }else {

        app_turn_off_pump();
        app_read_moisture_data(&moisture);
        f3d_delay_uS(100);
        app_read_battery_power(&battery);
        RTC_GetDate(RTC_Format_BIN, &CurrentDate);
        RTC_GetTime(RTC_Format_BIN, &CurrentTime);

        printf("Power: %d\n", battery);
        printf("Moisture: %d\n", moisture);
        printf("Date: %d:%d:%d\n", CurrentDate.RTC_Month, CurrentDate.RTC_Date,
                CurrentDate.RTC_Year);
        printf("Time: %d:%d:%d\n", CurrentTime.RTC_Hours, CurrentTime.RTC_Minutes, 
                CurrentTime.RTC_Seconds);

        f3d_delay_uS(100);
        app_prepare_nordic_packet(&moisture, 
            &battery, &CurrentDate, &CurrentTime, txdata);
        app_send_nordic_packet(txdata, rxdata);
        delay(100);

        app_enter_standby();
        while(1);
    }

    while(standby_count < SEND_INTERVAL_SECONDS) {

        f3d_led_all_on();

        // get time data
        RTC_GetDate(RTC_Format_BIN, &CurrentDate);
        RTC_GetTime(RTC_Format_BIN, &CurrentTime);

        // get analog data
        app_read_moisture_data(&moisture);
        f3d_delay_uS(100);
        app_read_battery_power(&battery);
        f3d_delay_uS(100);

        printf("Power: %d\n", battery);
        printf("Moisture: %d\n", moisture);
        printf("Date: %d:%d:%d\n", CurrentDate.RTC_Month, CurrentDate.RTC_Date,
                CurrentDate.RTC_Year);
        printf("Time: %d:%d:%d\n", CurrentTime.RTC_Hours, CurrentTime.RTC_Minutes, 
                CurrentTime.RTC_Seconds);
        printf("Standby count: %d\n", standby_count);

        //if (throwaway_count++ > 3) {
        app_prepare_nordic_packet(&moisture, 
                &battery, &CurrentDate, &CurrentTime, txdata);
        app_send_nordic_packet(txdata, rxdata);
        //}

        f3d_led_all_off();
        delay(1000);

    }

    app_turn_off_pump();
    app_read_moisture_data(&moisture);
    f3d_delay_uS(100);
    app_read_battery_power(&battery);
    f3d_delay_uS(100);
    app_prepare_nordic_packet(&moisture, 
            &battery, &CurrentDate, &CurrentTime, txdata);
    app_send_nordic_packet(txdata, rxdata);
    delay(100);
    app_enter_standby();
    while (1);
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
    /* Infinite loop */
    /* Use GDB to find out why we're here */
    while (1);
}
#endif

