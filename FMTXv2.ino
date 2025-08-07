/**
  @file    fmrx_demo.ino
  @author  www.elechouse.com
  @brief   example of FMRX_MODULE
  
    For this demo, input command format like &xxxx to set select band,
 (eg:if you want to set FM 95.6MHz,then input &0956 and send).
  
  @section  HISTORY
  
   Module // Arduino PRO/PRO MICRO    
    GND    ->   GND
    Vcc    ->   3.3V
    Tx     ->   PD3
    Rx     ->   PD2  
  
    Copyright (c) 2012 www.elechouse.com  All right reserved.
*/
#include <SoftwareSerial.h>
#include "FMTX.h"
SoftwareSerial HC12(PD3, PD2); // HC-12 TX Pin, HC-12 RX Pin

float ch;
float fm_freq = 98.6;  // Here set the default FM frequency
void setup(void)
{
  pinMode(PB5, OUTPUT);         //Blue  GPIO controlled LED on PB5
  pinMode(PB4, OUTPUT);      pinMode(PB3, OUTPUT);         //Blue  GPIO controlled LED on PB
  pinMode(PB2, OUTPUT);      pinMode(PB1, OUTPUT); 
  pinMode(PB0, OUTPUT);      //Blue  GPIO controlled LED on PB
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  HC12.begin(9600);               // Serial port to HC12
  Serial.print("Sompong's FM-TX 70 to 108 MHz.\r\n");
  
  /**
  Initial, set FM channel and select your area:
 USA
 EUROPE
 JAPAN
 AUSTRALIA
 CHINA
  */
  fmtx_init(fm_freq, EUROPE); 
  Serial.print("Channel:");
  Serial.print(fm_freq, 1);
  Serial.println("MHz");
  ch = fm_freq;
  HC12.write(fm_freq);      // Send that data to HC-12
}

void loop(void)
{
  /** check for data setting new frequency.  Users could input data from Serial monitor. Data 
    must start with '&' and followed by 4 numbers, such as &8000. The first two is the integer part
    of new frequency (Unit: MHz), and the last one is the decimal part. And the channel must between 70MHz
    and 108Mhz. For example,  &756 is 75.6MHz, and &666 is out of range.
    &0850  =  85 MHz, &0880 =  88 MHz, &1080 =  108 MHz
  */
  Serial.println(ch, 1);
  HC12.write(ch);      // Send that data to HC-12
  digitalWrite(PB5, !digitalRead(PB5));   delay(150);
  digitalWrite(PB4, !digitalRead(PB4));   delay(150);
  digitalWrite(PB3, !digitalRead(PB3));   delay(150);
  digitalWrite(PB2, !digitalRead(PB2));   delay(150);
  digitalWrite(PB1, !digitalRead(PB1));   delay(150);
  digitalWrite(PB0, !digitalRead(PB0));   delay(150);
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));   delay(150);
   if(Serial.available()){
    switch(Serial.read()){
      case '&':
        u8 i,buf[4];
         //float ch;
         i=0;
         delay(30);
         while(Serial.available()&&i<4){
           buf[i]=Serial.read();
           if (buf[i]<= '9' && buf[i]>= '0') { 
           i++;}
           else{
           i=0;
           break;
           }
         }
         if (i==4){
           ch = (buf[0]-'0')*100+(buf[1]-'0')*10+(buf[2]-'0')*1+0.1*(buf[3]-'0');
           if(ch>=70&&ch<=108){
             Serial.print("New Channel:");
             Serial.print(ch, 1);
             Serial.println("MHz");
             fmtx_set_freq(ch);
           }else{
             Serial.println("ERROR:Channel must be range from 70Mhz to 108Mhz.");
           }
         }else{
           Serial.println("ERROR:Input Format Error.");
         }
         
         while(Serial.available()){
           Serial.read();
         }
        break;
    }
  }
}