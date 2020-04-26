/* 	ds18b20_mfib.h -- H header file for handling the 1-wire DS18B20 digital thermometer
 *
 *  Created on: 	4.01.2017
 *  Author: 			Marek Frydrysiak <marek.frydrysiak@gmail.com>
 *	Description:
 *			Include the appropriate hal lib and select the 1-wire signal line port and pin.
 *			For more information, see the .c file!
 *
 * 	Copyright (C) 2017 Marek Frydrysiak
 *
 * 	This software may be modified and distributed under the terms
 * 	of the MIT license.  See the LICENSE file for details.
 *
 */

#ifndef _DS18B20_MFLIB_H_
#define _DS18B20_MFLIB_H_

#include <math.h>
#include "stm32f4xx_hal.h"			// <-- choose the hal lib appropriate for your device

#define TXRX_PORT	GPIOB					// <-- select the 1-wire signal line port (GPIOA or GPIOB or ...)
#define TXRX_PIN	GPIO_PIN_11		// <-- select the 1-wire signal line pin (GPIO_PIN_1 or GPIO_PIN_2 or ...)

#define READ_ROM_CMD_BYTE		0x33
#define SKIP_ROM_CMD_BYTE		0xCC

#define CONVERT_T_CMD					0x44
#define READ_SCRATCHPAD_CMD		0xBE

void DelayMicro(__IO uint32_t micros);
uint8_t ds18b20_init_seq(void);
uint64_t ds18b20_read_rom_cmd(void);
void ds18b20_send_function_cmd(uint8_t cmd);
void ds18b20_send_rom_cmd(uint8_t cmd);
float ds18b20_read_temp(void);
void ds18b20_write_byte(uint8_t data);
uint8_t ds18b20_read_bit(void);
uint8_t ds18b20_read_byte(void);

#endif /* _DS18B20_MFLIB_H_ */
