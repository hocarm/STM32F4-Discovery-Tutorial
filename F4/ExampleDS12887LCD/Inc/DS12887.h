/**
  ******************************************************************************
  * @file		DS12887.h
  * @author	    cuongabba
  * @date		6 February 2017
  * @note		Re-write form Peter Fleury AVR LCD library
  ******************************************************************************
  **/

#ifndef __DS12887_H
#define __DS12887_H

#ifdef __cplusplus
extern "C" {
#endif

/** Includes ---------------------------------------------------------------- */

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_gpio.h"

/** Port and pin definition for 12-bit mode ---------------------------------- */
// DS12887 control lines (must be on the same port) 



// ------------Pin definition-------------------------- //


// define pin high or low -----------------------

#define    DS12887_CS_LOW()                   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,GPIO_PIN_RESET);
#define    WR_LOW()                           HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);
#define    ALE_LOW()                          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,GPIO_PIN_RESET);
#define    RD_LOW()                           HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2,GPIO_PIN_RESET);

#define    DS12887_CS_HIGH()                  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3,GPIO_PIN_SET);
#define    WR_HIGH()                          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_SET);
#define    ALE_HIGH()                         HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,GPIO_PIN_SET);
#define    RD_HIGH()                          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2,GPIO_PIN_SET);

//---------- Define register DS12887 -------------

#define DS12C887_SECOND       0x00
#define DS12C887_MINUTE       0x02
#define DS12C887_HOUR         0x04
#define DS12C887_DAY          0x06
#define DS12C887_DATE         0x07
#define DS12C887_MONTH        0x08
#define DS12C887_YEAR         0x09
#define REGA                  0x0A
#define REGB                  0x0B
#define REGC                  0x0C


/** Public function prototypes ---------------------------------------------- */

void DS12C887_GetTime(unsigned char* Hour,unsigned char* Minute,unsigned char* Second);
void DS12C887_GetDate(unsigned char* DayWeek,unsigned char* DayMonth,unsigned char* Month,unsigned char* Year);
void DS12C887_SetDate(unsigned char day,unsigned char date,unsigned char month,unsigned char year);
void DS12C887_SetTime(unsigned char hour,unsigned char minute,unsigned char second);
unsigned char DS12C887_Read(unsigned char Regaddr);
void DS12C887_Init(void);
void DS12C887_Write(unsigned char Regaddr, unsigned char dat);
unsigned char Read_PortData(void);
void Write_PortData(unsigned char dat);
void configpin_GPIO(void);


#ifdef __cplusplus
}
#endif
#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/


