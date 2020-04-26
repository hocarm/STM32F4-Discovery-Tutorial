#ifndef __QVGA_TFT_H
#define __QVGA_TFT_H

#include "stm32f407xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stdint.h"



/* This is Library QVGA 2.2 Inches TFT SPI 240*320 For STM32F4 Microcontroller */
/* Author		: Xuan Thiep 	*/
/* Version	: 1.0 				*/
/* Date			: 21/07/2016  */


/* Define for connection, redefine if needed */

/* Define for module SPI */
extern SPI_HandleTypeDef hspi1;
#define SPI_HANDLE_TYPE_DEF hspi1 

/* Define for Chip Select Pin */
#define CS_PORT						GPIOA
#define CS_PIN						GPIO_PIN_4

/* Define for Data/Command Pin */
#define DC_PORT						GPIOD
#define DC_PIN						GPIO_PIN_12

/* Define for TFT Reset Pin */
#define TFT_RESET_PORT		GPIOD
#define TFT_RESET_PIN			GPIO_PIN_13

/* End Define For Connection */








/* Define private useful macro */
#define SPI_ENABLE			{HAL_GPIO_WritePin(CS_PORT,CS_PIN,GPIO_PIN_RESET);HAL_Delay(1);}
#define SPI_DISABLE     {HAL_GPIO_WritePin(CS_PORT,CS_PIN,GPIO_PIN_SET);HAL_Delay(1);}


/* Define For TFT */
#define LCD_W 240
#define LCD_H 320

/* Define For Color */
#define WHITE         	 	0xFFFF
#define BLACK         	 	0x0000	  
#define BLUE         	 		0x001F  
#define BRED             	0XF81F
#define GRED 			 				0XFFE0
#define GBLUE			 				0X07FF
#define RED           	 	0xF800
#define VIOLET	       	 	0xF81F
#define GREEN         	 	0x07E0
#define CYAN          	 	0x7FFF
#define YELLOW        	 	0xFFE0
#define BROWN 			 			0XBC40 
#define BRRED 			 			0XFC07 
#define GRAY  			 			0X8430 
#define DARKBLUE      	 	0X01CF	
#define LIGHTBLUE      	 	0X7D7C	
#define GRAYBLUE       	 	0X5458 
#define LIGHTGREEN     	 	0X841F 
#define LGRAY 			 			0XC618 
#define LGRAYBLUE        	0XA651 
#define LBBLUE           	0X2B12 


/* Define string mode for tft_puts */
#define TFT_STRING_MODE_NO_BACKGROUND		0x01
#define TFT_STRING_MODE_BACKGROUND			0x00

extern uint16_t BACK_COLOR, POINT_COLOR;






/* Declaring Function Prototype */

/***************************************************** User Function ************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/

void tft_init(void); 
void tft_clear(uint16_t Color);
void tft_draw_line(uint16_t row1, uint16_t column1, uint16_t row2, uint16_t column2);
void tft_draw_circle(uint16_t row,uint16_t column,uint8_t radian);
/* (row1,clumn1) ************************/
/* **************************************/
/* **************************************/
/* **************************************/
/* **************************************/
/* ***********************(row2,column2)*/
void tft_draw_rectangle(uint16_t row1, uint16_t column1, uint16_t row2, uint16_t column2);
void tft_fill(uint16_t row1,uint16_t column1,uint16_t row2,uint16_t column2,uint16_t color);
void tft_puts_image(const unsigned char* image_arr);



/* Note: If want to print digital use sprintf() function */
/* If digital is "dynamic" must use mode TFT_STRING_MODE_BACKGROUND */
void tft_puts8x16(uint16_t row,uint16_t column,int8_t *string,uint8_t TFT_STRING_MODE);	
void tft_puts14x24(uint16_t row,uint16_t column,int8_t *string,uint8_t TFT_STRING_MODE);
void tft_puts18x32(uint16_t row,uint16_t column,int8_t *string,uint8_t TFT_STRING_MODE);
void tft_puts26x48(uint16_t row,uint16_t column,int8_t *string,uint8_t TFT_STRING_MODE);




/******************************************************** Private Function ******************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/********************************************************************************************************************************/
/* Private Function Do Not Use In User Program */
void tft_add_set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void tft_write_data_8bit(uint8_t data);
void tft_write_data(uint16_t data);
void tft_write_register(int8_t data);
void tft_draw_point(uint16_t x,uint16_t y);
void tft_draw_point_big(uint16_t x,uint16_t y); 
void tft_putchar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode);
void tft_putchar26x48(uint16_t x,uint16_t y,uint8_t data,uint8_t mode);
void tft_putchar14x24(uint16_t x,uint16_t y,uint8_t data,uint8_t mode);
void tft_putchar18x32(uint16_t x,uint16_t y,uint8_t data,uint8_t mode);
#endif


