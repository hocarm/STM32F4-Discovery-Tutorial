#include "max7219.h"

/*	MAX7219_Init handles all the initialization
 * 	routines neccessary for the proper functioning
 * 	of the device. Call this function always
 * 	before you try to use any other functions
 * 	from this library.
 *
 * 	RETVAL: none
 * */
 // send data;
  //decode led7seg
 uchar const LEDcode[19]=
{
    0x7e,0x30,0x6d,0x79,0x33,0x5b,0x5f,0x70,0x7f,0x7b, //0..9
    0x77,0x1f,0x4e,0x3d,0x4f,0x47,0x67,0x3e,0xff       //a..f,P,U,all_on
};
 void MAX7219_SendByte (uchar dat)
{
    uchar i;

    for (i=0;i<8;i++)
    {
        if((dat&0x80)==0x80) HAL_GPIO_WritePin(GPIOE, MAX7219_DIN,GPIO_PIN_SET);
        else HAL_GPIO_WritePin(GPIOE, MAX7219_DIN,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, MAX7219_CLK,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOE, MAX7219_CLK,GPIO_PIN_SET);
        dat<<=1;
    }
}
// send address and dat;
void MAX7219_SendAddrDat (uchar addr, uchar dat)
{
    HAL_GPIO_WritePin(GPIOE, MAX7219_CS,GPIO_PIN_RESET);
    MAX7219_SendByte (addr);
    MAX7219_SendByte (dat);
    HAL_GPIO_WritePin(GPIOE, MAX7219_CS,GPIO_PIN_SET);
}
// setting max7219 init
void MAX7219_Init (void)
{
    MAX7219_SendAddrDat (0x0c,0x01); //normal operation
    MAX7219_SendAddrDat (0x0a,0x09); //intensity
    MAX7219_SendAddrDat (0x0b,0x07); //all digits on
    MAX7219_SendAddrDat (0x09,0x00); //decoding
    MAX7219_SendAddrDat (0x0f,0x00); //display test off
}

// xoa man hinh
void MAX7219_Clear(void)
{
    uchar i;
    for(i=8;i>0;i--) MAX7219_SendAddrDat(i,0x00);
}
// cat chuoi so;
void MAX7219_DisplayInt (uint val)
{
    uint a;

    a=val;
    MAX7219_SendAddrDat(0x08,LEDcode[(a/10000000)]);
    a=a%10000000;
    MAX7219_SendAddrDat(0x07,LEDcode[(a/1000000)]);
    a=a%1000000;
    MAX7219_SendAddrDat(0x06,LEDcode[(a/100000)]);
    a=a%100000;
    MAX7219_SendAddrDat(0x05,LEDcode[(a/10000)]);
    a=a%10000;
    MAX7219_SendAddrDat(0x04,LEDcode[(a/1000)]);
    a=a%1000;
    MAX7219_SendAddrDat(0x03,LEDcode[(a/100)]);
    a=a%100;
    MAX7219_SendAddrDat(0x02,LEDcode[(a/10)]);
    a=a%10;
    MAX7219_SendAddrDat(0x01,LEDcode[a]);
}



