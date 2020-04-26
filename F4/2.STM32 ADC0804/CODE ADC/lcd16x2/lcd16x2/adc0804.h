/**
  ******************************************************************************
  * @file		DS12887.h
  * @author	    cuongabba
  * @date		6 February 2017
  * @note		Re-write form Peter Fleury AVR LCD library
  ******************************************************************************
  **/

#ifndef __ADC0804_H
#define __ADC0804_H

#ifdef __cplusplus
extern "C" {
#endif

/** Includes ---------------------------------------------------------------- */
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

static uint8_t value ;
/** Port and pin definition for 12-bit mode ---------------------------------- */
// DS12887 control lines (must be on the same port) 

#define ADC0804_RCC_GPIO_CONTROL	RCC_APB2Periph_GPIOA
#define ADC0804_GPIO_CONTROL			    GPIOA
//
// ------------Pin definition-------------------------- //


// define pin high or low -----------------------

#define    CS_LOW          GPIO_ResetBits(ADC0804_GPIO_CONTROL, GPIO_Pin_2);
#define    WR_LOW          GPIO_ResetBits(ADC0804_GPIO_CONTROL, GPIO_Pin_4);                     
#define    RD_LOW          GPIO_ResetBits(ADC0804_GPIO_CONTROL, GPIO_Pin_3);


#define    CS_HIGH         GPIO_SetBits(ADC0804_GPIO_CONTROL, GPIO_Pin_2);
#define    WR_HIGH         GPIO_SetBits(ADC0804_GPIO_CONTROL, GPIO_Pin_4); 
#define    RD_HIGH         GPIO_SetBits(ADC0804_GPIO_CONTROL, GPIO_Pin_3); 




/** Public function prototypes ---------------------------------------------- */

void Config_ADC_GPIO(void);
unsigned char ADC0804_Read(void);

#ifdef __cplusplus
}
#endif
#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/


