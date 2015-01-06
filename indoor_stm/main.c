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
#include <lsi.h>
#include <schedule.h>
#include <ds_nordic.h>
#include <nrf24l01.h>
#include <math.h>
#include <stdio.h>




volatile int packet_received = 0;
char rxdata[32];
char txdata[32] = { "Submission received" };
int result;
int power=100;
uint8_t pump;
int m_buffer[10]={0,0,0,0,0,0,0,0,0,0};
int m_update=0;

int rtc_set=0;

char t_buffer[5];
char d_buffer[5];


volatile int lcd_update=0;
volatile int lcd_update_counter=0;

int read_moisture_data(char *data) {
    int result = 0;
    result |= (int) data[0];
    result = result << 8;
    result |= (int) data[1];
    result = result << 8;
    result |= (int) data[2];
    result = result << 8;
    result |= (int) data[3];
    return result;
}

uint8_t read_pump_data(char *data){
  uint8_t result;
  result=data[4];
  return result;
}

int read_battery_data(char *data) {
    int result = 0;
    result |= (int) data[5];
    result = result << 8;
    result |= (int) data[6];
    result = result << 8;
    result |= (int) data[7];
    result = result << 8;
    result |= (int) data[8];

    //printf("befor: %d\n",result);
    result = (int) ( (float) 100* (result) / 4096.00 );
    //printf("after: %d,\n",result);
    
    return result;
}


void read_time_data(char *data,uint8_t *linebuffer){
  int i=0;
  for (i=0;i<6;i++){
    linebuffer[i] = data[i+9];
  }

  
}


void read_rx() {
    nrf24l01_read_rx_payload(rxdata, 32);
    nrf24l01_irq_clear_all();
    f3d_delay_uS(130);

    // Payload Translation
    //printf("Base Data: ");
    //printf("%d\n",read_moisture_data(rxdata));
    /*
    for (index=0;index<32;index++) {
        printf("%c",rxdata[index]);
    }
    printf("\n");

    for (index=0;index<32;index++) {
        if ((rxdata[index] >= 'a') && (rxdata[index] <= 'z')) {
            rxdata[index]-=32;
        }
    }
    
    printf("Base: receive character: %c\n",rxdata);
    // rxdata-=32; 
    printf("Base: upcase: %c\n",rxdata);
    
    printf("Base Sending uppercase char: %c\n", rxdata);
    */
    printf("Sending %s\n", txdata);
    nrf24l01_set_as_tx();
    nrf24l01_write_tx_payload(txdata, 32, true);
    while(!(nrf24l01_irq_pin_active() && (nrf24l01_irq_tx_ds_active() || nrf24l01_irq_max_rt_active())));
    nrf24l01_irq_clear_all();
    nrf24l01_set_as_rx(true);
    nrf24l01_clear_flush();

}

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

void lcd_draw_base(){
  f3d_lcd_fillScreen(WHITE);
  f3d_lcd_drawString(1,5,"TIME:",BLACK,WHITE);
  f3d_lcd_drawString(65,5,"DATE:",BLACK,WHITE);

  //drawing chart
  int h,v;
  //the chart is 100x100 pixels
  for(h=15;h<115;h++){
    f3d_lcd_drawPixel(h,120,BLACK);
  }
  for(v=120;v>20;v--){
    f3d_lcd_drawPixel(15,v,BLACK);
  }
  f3d_lcd_drawString(5,115,"L",BLACK,WHITE);
  f3d_lcd_drawString(5,65,"M",BLACK,WHITE);
  f3d_lcd_drawString(5,20,"H",BLACK,WHITE);
  f3d_lcd_drawString(105,125,"NOW",BLACK,WHITE);

  //init pump status will be red
  draw_circle(8,10,150,RED);
  f3d_lcd_drawString(20,145,"PUMP",BLACK,WHITE);

  //battery status drawing
  int i,k;
  for(i=75;i<120;i++){
    f3d_lcd_drawPixel(i,155,BLACK);
  }
  for(i=75;i<120;i++){
    f3d_lcd_drawPixel(i,140,BLACK);
  }
  for(k=140;k<156;k++){
    f3d_lcd_drawPixel(75,k,BLACK);
  }
  for(k=140;k<156;k++){
    f3d_lcd_drawPixel(120,k,BLACK);
  }

  for(i=120;i<125;i++){
    f3d_lcd_drawPixel(i,143,BLACK);
  }
  for(i=120;i<125;i++){
    f3d_lcd_drawPixel(i,152,BLACK);
  }
  for(k=143;k<153;k++){
    f3d_lcd_drawPixel(125,k,BLACK);
  }
}

void pump_draw(char pump){
  //status for water pump; green is on; red is stop
  uint16_t color=RED;
  if(pump){
    color=GREEN;
  }
  else{
    color=RED;
  }
  draw_circle(8,10,150,color);
}



void battery_color(int power){
  if(power>100){
    power=100;
  }

  if(power<0){
    power=0;
  }
  int x,y;
  uint16_t color=GREEN;
  //int x_limit=120;
  float x_limit=(power/100.00)*44+76;
  char buffer[3];
  char first = (char)(((int)'0')+power/100);
  char second = (char)(((int)'0')+power/10);
  char third = (char)(((int)'0')+power%10);
  //printf("%c %c %c\n",first, second, third);

  if(power>=80){color=GREEN;} 
  else if(power>=60){color=YELLOW;}
  else if(power>=40){color=YELLOW;}
  else if(power>=20){color=RED;} 
  else {color=RED;}
 
  //////////flush
  for(x=76;x<120;x++){
    for(y=141;y<155;y++){
      f3d_lcd_drawPixel(x,y,WHITE);
    }
  }
  for(x=121;x<125;x++){
    for(y=144;y<152;y++){
      f3d_lcd_drawPixel(x,y,WHITE);
    }
  }
  //////////////////////////////////////

  //batery blackground color
  for(x=76;x<x_limit;x++){
    for(y=141;y<155;y++){
      f3d_lcd_drawPixel(x,y,color);
    }
  }
 
  //f3d_lcd_drawString(87,145,"100",BLACK,color);
  if(first=='1'){
    f3d_lcd_drawString(87,145,"100%",BLACK,color);
    for(x=121;x<125;x++){
      for(y=144;y<152;y++){
	f3d_lcd_drawPixel(x,y,color);
      }
    }
  }
  else if(first=='0' && second!='0'){
    f3d_lcd_drawChar(96,145,second,BLACK,color);
    f3d_lcd_drawChar(102,145,third,BLACK,color);
    f3d_lcd_drawChar(112,145,'%',BLACK,color);
  }
  else{
    f3d_lcd_drawChar(102,145,third,BLACK,color);
    f3d_lcd_drawChar(112,145,'%',BLACK,color);
  }
}
 

//moisture from 0-2000,0-600 is LOW,601-1200 is MED, >1600 is HIGH
//the whole graph show ten data,it will pop the oldest one the get the newest data


void graph(int m_buffer[],int flush){
  //x start at 15 to 115
  //y start at 20 to 120
  int i,k,z;
  int ox=20;//15+5
  int oy=15;//20-5
  int dot_y[10];
  float dis;

  for(i=0;i<=9;i++){
    dot_y[i]=oy+(100-(int)(m_buffer[i]/20));
    ox+=10;
    oy=15;
  }
  
  //reset ox to start point for the function below
  ox=20;

  //joining dots with line
  for(i=0;i<9;i++){
    //the next one is higher than last one
    if(dot_y[i]-dot_y[i+1]>0){
      for(k=0;k<10;k+=1){
	dis=(dot_y[i]-dot_y[i+1])/10.00;
	if(flush){
	  draw_circle(2,ox+k,dot_y[i]-(dis*k),WHITE);
	}

	else{
	  draw_circle(2,ox+k,dot_y[i]-(dis*k),BLACK);  
	}
      }	  
    }

    else{
      for(k=0;k<10;k+=1){
	dis=(dot_y[i+1]-dot_y[i])/10.00;
	if(flush){
	  draw_circle(2,k+ox,dot_y[i]+(dis*k),WHITE);
	}
	
	else{
	  draw_circle(2,k+ox,dot_y[i]+(dis*k),BLACK);
	}
      }
    }
    ox+=10;
  }

  //reset back to start point for the following function
  ox=20;

  for(i=0;i<=9;i++){
    if(flush){
      draw_circle(4,ox,dot_y[i],WHITE);
    }
    else{
      draw_circle(4,ox,dot_y[i],BLUE);
    }
    ox+=10;
    oy=15;
  }
}


void time_flush(){
  int x,y;
  for(x=30;x<65;x++){
    for(y=0;y<14;y++){
      f3d_lcd_drawPixel(x,y,WHITE);
    }
  }
  
  for(x=95;x<128;x++){
    for(y=0;y<14;y++){
      f3d_lcd_drawPixel(x,y,WHITE);
    }
  } 
}

void time_update(RTC_TimeTypeDef *t,RTC_DateTypeDef *d,uint8_t *linebuffer){
  RTC_TimeStructInit(t);
  RTC_DateStructInit(d);

  d->RTC_Month=linebuffer[0];
  d->RTC_Date=linebuffer[1];
  d->RTC_Year=linebuffer[2];
  t->RTC_Hours=linebuffer[3];
  t->RTC_Minutes=linebuffer[4];
  t->RTC_Seconds=linebuffer[5];
  
  PWR_BackupAccessCmd(ENABLE);
  RTC_SetTime(RTC_Format_BIN, t);
  RTC_SetDate(RTC_Format_BIN, d);
  PWR_BackupAccessCmd(DISABLE);

  rtc_set=1;

}



void draw_time(RTC_TimeTypeDef *t,RTC_DateTypeDef *d,char t_buffer[], char d_buffer[]){
  //char t_buffer[5];
  //char d_buffer[5];

  uint8_t month=d->RTC_Month;
  uint8_t day=d->RTC_Date;
  uint8_t year=d->RTC_Year;
  uint8_t hour=t->RTC_Hours;
  uint8_t minute=t->RTC_Minutes;
  uint8_t second=t->RTC_Seconds;
  

  if(t_buffer[4]!=((char)(((int)'0')+minute%10))){
    time_flush();
  }

  if(hour>9){
    t_buffer[0]=(char)(((int)'0')+hour/10);
    t_buffer[1]=(char)(((int)'0')+hour%10);
  }
  else {
    t_buffer[0]='0';
    t_buffer[1]=(char)(((int)'0')+hour%10);
  }

  t_buffer[2]=':';

  if(minute>9){
    t_buffer[3]=(char)(((int)'0')+minute/10);
    t_buffer[4]=(char)(((int)'0')+minute%10);
  }
  else {
    t_buffer[3]='0';
    t_buffer[4]=(char)(((int)'0')+minute%10);
  }
  


  if(month>9){
    d_buffer[0]=(char)(((int)'0')+month/10);
    d_buffer[1]=(char)(((int)'0')+month%10);
  }
  else{
    d_buffer[0]='0';
    d_buffer[1]=(char)(((int)'0')+month%10);
  }
  
  d_buffer[2]='/';

  if(day>9){
    d_buffer[3]=(char)(((int)'0')+day/10);
    d_buffer[4]=(char)(((int)'0')+day%10);
  }
  else{
    d_buffer[3]='0';
    d_buffer[4]=(char)(((int)'0')+day%10);
  }

  f3d_lcd_drawString(30,5,t_buffer,BLACK,WHITE);
  f3d_lcd_drawString(95,5,d_buffer,BLACK,WHITE);
 
 

}


//update the m_buffer if receive a new data, pop the oldest one and push the newest one
void m_buffer_update(int result,int m_buffer[]){
  int i;
  for(i=0;i<9;i++){
    m_buffer[i]=m_buffer[i+1];
  }
  m_buffer[9]=result;
}


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

    RTC_init();

    RTC_TimeTypeDef t;
    RTC_DateTypeDef d;
    
    uint8_t linebuffer[100];
   
    

    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    RTC_GetTime(RTC_Format_BIN,&t);
    RTC_GetDate(RTC_Format_BIN,&d);

    nrf24l01_initialize_debug(true, 32, true); // Enhanced Shockburst, Auto Ack turned on
    nrf24l01_clear_flush();

    


    lcd_draw_base();
   
    graph(m_buffer,0);
 

    while (1) {
  
      RTC_GetTime(RTC_Format_BIN,&t);
      RTC_GetDate(RTC_Format_BIN,&d);
      printf("%d:%d:%d %d/%d/%d\n",t.RTC_Hours,t.RTC_Minutes,t.RTC_Seconds,d.RTC_Month,d.RTC_Date,d.RTC_Year + 2000);
      
      //time_flush();
      draw_time(&t,&d,t_buffer,d_buffer);

      if (packet_received){
            read_rx();
	    m_update=1;
	    result= read_moisture_data(rxdata);
	    pump=read_pump_data(rxdata);
	    power=read_battery_data(rxdata);
	    read_time_data(rxdata,linebuffer);
	    graph(m_buffer,1);//for flush the graph
	    m_buffer_update(result,m_buffer);
      }

      if(m_update){
	
	graph(m_buffer,0);
	battery_color(power);
	pump_draw(pump);
	if(!rtc_set){
	  time_update(&t,&d,linebuffer);
	}
	m_update=0;
      }
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
