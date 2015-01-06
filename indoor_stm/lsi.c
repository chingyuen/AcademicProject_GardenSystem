#include <lsi.h>
#include <stdio.h>

void RTC_init(void) {
  RTC_TimeTypeDef RTC_TimeStructure;
  RTC_DateTypeDef RTC_DateStructure;
  RTC_InitTypeDef RTC_InitStructure;
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  PWR_BackupAccessCmd(ENABLE); // Enable access to write to the RTC Registers
  RCC_BackupResetCmd(ENABLE);
  RCC_BackupResetCmd(DISABLE);

  PWR_BackupAccessCmd(ENABLE);

  /* LSI used as RTC source clock*/
  RCC_LSICmd(ENABLE);
  
  /* Wait till LSI is ready */
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

void getline(char *buf) {
  char c;
  c=getchar();
  while ((c!='\n') && (c!='\r')) {
    *buf++ = c;
    putchar(c);
    c = getchar();
  }
  *buf = 0;
}

int rtc_settime (uint8_t *buf) {
  enum {HOUR,MINUTE,SECOND,MONTH,DAY,YEAR};

  RTC_TimeTypeDef t;
  RTC_DateTypeDef d;

  uint8_t *tok_start = buf;
  uint8_t *tok_end = buf;
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
      d.RTC_Year=atoi(tok_start)-2000;
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

