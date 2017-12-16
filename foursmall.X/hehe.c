
#include <xc.h>

#pragma config OSC = INTIO67   // Oscillator Selection bits (HS oscillator)
#pragma config WDT = OFF  // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRT = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON  // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF   // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF   // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)

#include <pic18.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "ws2812.h"
#include <pic18f4520.h>

// the pin that the LEDs are on. I don't like this being in here
// but timing is so tight, it is hard to put it elsewhere
#define PIN PORTDbits.RD0


unsigned long int bitflip(unsigned char b);
long int temp;
unsigned char tmp1;
unsigned char tmp2;
unsigned char tmp3;
ws2812_ptr          led;
long int ledlength;
int flag;
int lednum;
long int lednum_24;
int isFlow=0;
int ledmax;

void led_send(int front) {
    long int front_24 = front * 24;
    int buf = 24;
    long int val = (long int)tmp1;
    long int back_24 = (ledlength - front ) * 24;
    
    for(long int i = 0; i < front_24; ++i){
        PIN = 1;
        PIN = 0;
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
        NOP();
    }
    
    for(long int i = 0; i < lednum_24 ; i++ ){
        //if(lednum!=20) lednum++;
        if(buf== 0){
            buf = 24;
            val = (long int)tmp1;
        }
        else if(buf== 16){
          //  buf = 24;
            val = (long int)tmp2;
        }
        else if(buf== 8){
            val = (long int)tmp3;
        }
        //val=1;
        if (val & 1) {
            PIN = 1;
            NOP();
            NOP();
            NOP();
             NOP();
        NOP();
        NOP();
            PIN = 0;
           
        } else {
            PIN = 1;
            PIN = 0;
            NOP();
            NOP();
            NOP();
            NOP();
        NOP();
        NOP();
        }    
        val >>= (unsigned char)1;
        buf--;
    }
   
    for(long int i = 0; i < back_24; ++i){
        PIN = 1;
        PIN = 0;
        NOP();
        NOP();
        NOP();
       NOP();
        NOP();
        NOP();
    }
}

// reverse the bits in a char
unsigned long int bitflip(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return (unsigned char)b;
}

void init(){
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 1;
    OSCTUNEbits.PLLEN=1;
    
    ledlength=300;
    lednum=1;
    lednum_24= lednum*24;
    ledmax=300;
    
    led.r = 1;
    led.g = 1;
    led.b = 100;

    isFlow=0;
    
    TRISDbits.RD0 = 0;
}

void main() {    
    init();
    tmp1=bitflip(led.g);
    tmp2=bitflip(led.r);
    tmp3=bitflip(led.b);
    //temp = (bitflip(led.b) << 16) + (bitflip(led.r) << 8) + (bitflip(led.g));
    int front = 0;

    while(1) {
        CLRWDT();
        GIE = 0; while (GIE);
        if(front > ledlength)
        {    
             front=0;
             isFlow=0;
             lednum=1;
             lednum_24= lednum*24;
        }
         led_send(front);
         if(isFlow){
             front++;
         }
         if(lednum!=ledmax) 
         {
             lednum++;
             lednum_24= lednum*24;
         }
        if(lednum==ledmax) 
        {
            isFlow=1;
        }
        //delay part
        int k = 1 , l = 5;
        while( k > 0 ) {
            l = 5;
            while( l > 0) {
                l--;
            }
            k--;
        }
       
        GIE = 1; while (!GIE);
    }
}
