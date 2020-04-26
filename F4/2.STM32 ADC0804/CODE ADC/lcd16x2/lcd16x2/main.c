
#include "delay.h"
#include "lcd16x2.h"
#include <stdio.h>
#include "adc0804.h"

unsigned char adc;
unsigned int voltage;
char buf[20];

int main(void)
{
	// Delay initialization
	DelayInit();
	// LCD initialization
	lcd16x2_init(LCD16X2_DISPLAY_ON_CURSOR_OFF_BLINK_OFF);
	Config_ADC_GPIO();
	while (1)
	{//adc/100+0x30,adc%100/10+0x30,adc%10+0x30
		adc = ADC0804_Read();
		lcd16x2_gotoxy(0,0);
		sprintf(buf,"Temp = %d *C",(int)adc);
		lcd16x2_puts(buf);
		//DelayMs(500);
		// Clear display-
		//lcd16x2_clrscr();
		
	}
}

