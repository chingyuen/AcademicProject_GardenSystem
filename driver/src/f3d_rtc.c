/* f3d_rtc.c --- 
 * 
 * Filename: f3d_rtc.c
 * Description: 
 * Author: Ching Yuen Ng/ Brian Rak
 * Maintainer: 
 * Created: Fri Feb 21 2014
 * Last-Updated: 02/21/2014
 *           By: brak/ching
 *     Update #: 1
 * Keywords: 
 * Compatibility: 
 * 
 */

#include <f3d_rtc.h>
#include <stdio.h>


volatile int standby_flag = 0;

/*
 * Inits the LSE RTC
 */
void RTC_init(void) {
    RTC_TimeTypeDef RTC_TimeStructure;
    RTC_DateTypeDef RTC_DateStructure;
    RTC_InitTypeDef RTC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_BackupAccessCmd(ENABLE);
    RCC_BackupResetCmd(ENABLE);
    RCC_BackupResetCmd(DISABLE);
    RCC_LSEDriveConfig(RCC_LSEDrive_High);
    RCC_LSEConfig(RCC_LSE_ON);
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    RCC_RTCCLKCmd(ENABLE);

    PWR_BackupAccessCmd(ENABLE);
    RTC_WriteProtectionCmd(DISABLE); // Disable the RTC's write protection
    RTC_EnterInitMode();
    RTC_WaitForSynchro();

    RTC_StructInit(&RTC_InitStructure);
    RTC_Init(&RTC_InitStructure);

    RTC_DateStructInit(&RTC_DateStructure);

    RTC_DateStructure.RTC_Month   = RTC_Month_May;
    RTC_DateStructure.RTC_Date    = 28;
    RTC_DateStructure.RTC_Year    = 14;
    
    RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

    RTC_TimeStructInit(&RTC_TimeStructure);
    RTC_TimeStructure.RTC_Hours   = 22;
    RTC_TimeStructure.RTC_Minutes = 21;
    RTC_TimeStructure.RTC_Seconds = 0;

    RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);

    RTC_ExitInitMode();
    RTC_WriteProtectionCmd(ENABLE);
    PWR_BackupAccessCmd(DISABLE);
}

void RTC_LSI_init(void) {
    RTC_TimeTypeDef RTC_TimeStructure;
    RTC_DateTypeDef RTC_DateStructure;
    RTC_InitTypeDef RTC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    PWR_BackupAccessCmd(ENABLE); // Enable access to write to the RTC Registers
    RCC_BackupResetCmd(ENABLE);
    RCC_BackupResetCmd(DISABLE);

    PWR_BackupAccessCmd(ENABLE);

    // LSI used as RTC source clock
    RCC_LSICmd(ENABLE);

    // Wait till LSI is ready 
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
    RCC_RTCCLKCmd(ENABLE);
    RTC_WaitForSynchro();


    RTC_StructInit(&RTC_InitStructure); // Set the structure members to their default values
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
    RTC_InitStructure.RTC_AsynchPrediv = 88;
    RTC_InitStructure.RTC_SynchPrediv = 470;
    RTC_Init(&RTC_InitStructure);


    RTC_DateStructure.RTC_Year = 13;
    RTC_DateStructure.RTC_Month = RTC_Month_November;
    RTC_DateStructure.RTC_Date = 7;
    RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Thursday;
    RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

    RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
    RTC_TimeStructure.RTC_Hours = 0x09;
    RTC_TimeStructure.RTC_Minutes = 0x05;
    RTC_TimeStructure.RTC_Seconds = 0x00;
    RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);

}


/**
 * @brief  This function configures the system to enter Standby mode with RTC
 *         clocked by LSI for current consumption measurement purpose.
 *         STANDBY Mode with RTC clocked by LSI
 *         ========================================
 *           - RTC Clocked by LSI
 *           - IWDG OFF
 *           - Automatic Wakeup using RTC
 * @param  None
 * @retval None
 */

void f3d_standby_init(void)
{

    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    RTC_InitTypeDef RTC_InitStructure;

    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG , ENABLE);


    /* Allow access to RTC */
    PWR_BackupAccessCmd(ENABLE);
    RTC_WriteProtectionCmd(DISABLE);

    //Enable the LSE OSC
    RCC_LSEConfig(RCC_LSE_ON);

    //wait until LSE is ready
    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET){}

    //Select RTC clk source
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    //SynchPrediv = 0xFF;
    //AsynchPrediv = 0x7F;

    /* Enable the RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();

    // 32768Hz / 2 - 1 = 0x3FFF
    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
    // 32768Hz / 1 - 1 = 0x7FFF
    RTC_InitStructure.RTC_SynchPrediv   =  0xFF; 
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
    RTC_Init(&RTC_InitStructure);


    /* EXTI configuration *******************************************************/
    
    EXTI_ClearITPendingBit(EXTI_Line20);
    EXTI_InitStructure.EXTI_Line = EXTI_Line20;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);



    // Enable the RTC Wakeup Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_SetPriority(RTC_WKUP_IRQn, 0x04);
    //RTCCLK=32768Hz ; div=16  =>2048Hz

    //div 256 =>8Hz  ~ 125ms

    RTC_ClearITPendingBit(RTC_IT_WUT);
    EXTI_ClearITPendingBit(EXTI_Line20);
    // Enable the RTC Wakeup Interrupt
    
    RTC_ITConfig(RTC_IT_WUT, ENABLE);
    
    /* Enable Wakeup Counter */
    
    
    RTC_WakeUpCmd(DISABLE);
    RTC_SetWakeUpCounter(0x1F);
    //RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div2);
    RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
    RTC_ExitInitMode();
    RTC_WriteProtectionCmd(ENABLE);
    RTC_WakeUpCmd(ENABLE);
}


int rtc_settime (char *buf) {
    enum {HOUR,MINUTE,SECOND,MONTH,DAY,YEAR};

    RTC_TimeTypeDef t;
    RTC_DateTypeDef d;

    char *tok_start = buf;
    char *tok_end = buf;
    int field = HOUR;

    RTC_TimeStructInit(&t);
    RTC_DateStructInit(&d);

    while (field<=YEAR) {
        while ((*tok_end != ',') && (*tok_end)) {
            tok_end++;
        }
        *tok_end = 0;
        switch (field++) {
            case HOUR:
                t.RTC_Hours=atoi(tok_start);
                printf("hour = %d\n",t.RTC_Hours);
                break;
            case MINUTE:
                t.RTC_Minutes=atoi(tok_start);
                printf("min = %d\n",t.RTC_Minutes);
                break;
            case SECOND:
                t.RTC_Seconds=atoi(tok_start);
                printf("sec = %d\n",t.RTC_Seconds);
                break;
            case MONTH:
                d.RTC_Month=atoi(tok_start);
                printf("month = %d\n",d.RTC_Month);
                break;
            case DAY:
                d.RTC_Date=atoi(tok_start);
                printf("day = %d\n",d.RTC_Date);
                break;
            case YEAR:
                d.RTC_Year=atoi(tok_start) - 2000;
                printf("year = %d\n",d.RTC_Year);
                break;
        }
        tok_end++;
        tok_start=tok_end;
    }

    PWR_BackupAccessCmd(ENABLE);
    RTC_SetTime(RTC_Format_BIN, &t);
    RTC_SetDate(RTC_Format_BIN, &d);
    PWR_BackupAccessCmd(DISABLE);
    return (0);
}

void RTC_WKUP_IRQHandler(void) {
    standby_flag = 1;
    RTC_ClearITPendingBit(RTC_IT_WUT);
    EXTI_ClearITPendingBit(EXTI_Line20);
}
