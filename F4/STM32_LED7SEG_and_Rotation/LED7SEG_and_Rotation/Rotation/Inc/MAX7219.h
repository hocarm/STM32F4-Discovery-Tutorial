#ifndef		__MAX7219_H
#define		__MAX7219_H

#include "stm32f4xx_hal.h"

  #define MAX7219_CLK   GPIO_PIN_1    //PORTE
  #define MAX7219_DIN   GPIO_PIN_3    //PORTE
  #define MAX7219_CS    GPIO_PIN_2    //PORTE
  #define LED_BEAT      GPIO_PIN_13   //PORTD
	// define  variable
  #define uchar unsigned char
  #define ushort unsigned short
  #define ulong unsigned long
  #define uint unsigned int
		

void MAX7219_SendAddrDat (uchar addr, uchar dat);
void MAX7219_SendByte (uchar dat);
void MAX7219_Init (void);
void MAX7219_Clear(void);
void MAX7219_DisplayInt (uint val);

#endif
