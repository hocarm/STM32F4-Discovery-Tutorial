/**
  ******************************************************************************
  * @file		DS12887.h
  * @author	cuongabba
  * @date		6 February 2018
	* @note		Re-write form Peter Fleury DS12887 library
  ******************************************************************************
  */

/** Includes ---------------------------------------------------------------- */
#include "adc0804.h"
#include <stdio.h>
#include <math.h>
GPIO_InitTypeDef GPIOInitStruct;

void Config_ADC_GPIO(void)
{
	// GPIO clock for control and data lines
	RCC_APB2PeriphClockCmd(ADC0804_RCC_GPIO_CONTROL, ENABLE); // anble tong
	// Configure I/O for control lines as output
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3 |GPIO_Pin_4;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(ADC0804_GPIO_CONTROL, &GPIOInitStruct);
	
		RCC_APB2PeriphClockCmd(ADC0804_RCC_GPIO_CONTROL, ENABLE); // anble tong
	// Configure I/O for control lines as output
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(ADC0804_GPIO_CONTROL, &GPIOInitStruct);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // anble tong
	// Configure I/O for control lines as output
  GPIOInitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIOInitStruct.GPIO_Mode = GPIO_Mode_IPD; //GPIO_Mode_IPU;
  GPIOInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIOInitStruct);
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	GPIOInitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
//	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IPU;
//	GPIOInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
//	
//	GPIO_Init(GPIOB, &GPIOInitStruct);
	
}
unsigned char Read_ADC_PIN(void)
{
  unsigned char dat;
  dat=0;
  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)) dat|=0x80; 
  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)) dat|=0x40;
  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)) dat|=0x20;
  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)) dat|=0x10;
  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)) dat|=0x08;
  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2)) dat|=0x04;
  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)) dat|=0x02;
  if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)) dat|=0x01; 

  return(dat);
}
unsigned char ADC0804_Read(void)
{
	unsigned char DATA;

	// Chon chip
 	CS_LOW;

	// Tao xung bat dau chuyen doi
	WR_LOW;
	RD_HIGH;
	WR_HIGH;
 WR_HIGH;
	// Doi cho den khi chuyen doi xong
	while(GPIO_ReadInputDataBit(ADC0804_GPIO_CONTROL, GPIO_Pin_5)); //READ_INTR=1

	// Doc gia tri sau khi chuyen doi
	RD_LOW;
	DATA = Read_ADC_PIN();
	//RD_HIGH;
  //CS_HIGH;
	return DATA;
}
	