/* 	ds18b20_mfib.c -- C source file for handling the 1-wire DS18B20 digital thermometer
 *
 *  Created on: 	4.01.2017
 *  Author: 			Marek Frydrysiak <marek.frydrysiak@gmail.com>
 *	Description:
 *			
 *			This C source file provides very basic functionalities for controlling
 *			the DS18B20 digital thermometer. So far it handles only one device.
 *
 *			In order to read the temperature measurement, the following sequence of
 *			functions must be applied:
 *
 *				ds18b20_init_seq();
 *				ds18b20_send_rom_cmd(SKIP_ROM_CMD_BYTE);
 *				ds18b20_send_function_cmd(CONVERT_T_CMD);
 *
 *				delay_us(100);
 *
 *				ds18b20_init_seq();
 *				ds18b20_send_rom_cmd(SKIP_ROM_CMD_BYTE);
 *				ds18b20_send_function_cmd(READ_SCRATCHPAD_CMD);
 *				temp_f = ds18b20_read_temp();	// returns float value
 *
 *			User can adapt the delay_us function in a way she/he likes. In this case 
 *			the time_us variable is decremented in SysTick handler every 1us. This is 
 *			obviously wasting of uC resources, but timings must be obey.
 *
 *			This code is prepared to work with the HAL STM32 libraries. The only thing
 *			the user is obliged to do, except the time_us variable, is to edit the .h file,
 *			i.e. to define 1-wire signal PIN and PORT and to include appropriate HAL 
 *			header file.
 *
 *			1-wire signal line should be pulled-up to VCC of the device with a 4.7k resistor.
 *			Moreover, the signal line PIO should be configured in the output open-drain mode.
 *
 *
 * 	Copyright (C) 2017 Marek Frydrysiak
 *
 * 	This software may be modified and distributed under the terms
 * 	of the MIT license.  See the LICENSE file for details.
 *
 */

#include "ds18b20_mflib.h"

extern volatile uint32_t time_us;

/**
 * Inserts a delay (in us).
 *
 * @param 	t: Time in us.
 * @return 	None.
 */
void delay_us(uint32_t t)
{
	time_us = t;
	while (time_us) {
		/* do nothing */
	}
}

/**
 * Initialises communication with the DS18B20 device.
 *
 * @param 	None.
 * @return 	1 if succeed, 0 if failed.
 */
uint8_t ds18b20_init_seq(void)
{
	HAL_GPIO_WritePin(TXRX_PORT, TXRX_PIN, GPIO_PIN_RESET);
	delay_us(600);
	HAL_GPIO_WritePin(TXRX_PORT, TXRX_PIN, GPIO_PIN_SET);
	delay_us(100);
	if (HAL_GPIO_ReadPin(TXRX_PORT, TXRX_PIN) == GPIO_PIN_RESET) {
		delay_us(500);
		return 1;
	} else {
		delay_us(500);
		return 0;
	}
}

/**
 * Sends the read rom command.
 *
 * @param 	None.
 * @return 	The lasered rom code.
 */
uint64_t ds18b20_read_rom_cmd(void)
{
	uint8_t crc, family_code;
	uint64_t lasered_rom_code, serial_num;

	ds18b20_write_byte(READ_ROM_CMD_BYTE);

	family_code = ds18b20_read_byte();
	serial_num = ds18b20_read_byte();
	serial_num |= (ds18b20_read_byte() << 8);
	serial_num |= (ds18b20_read_byte() << 16);
	crc = ds18b20_read_byte();

	lasered_rom_code = (crc << 24) | (serial_num << 8) |  family_code;

	return lasered_rom_code;

}

/**
 * Sends the function command.
 *
 * @param 	cmd: Number of the function command
 * @return  None.
 */
void ds18b20_send_function_cmd(uint8_t cmd)
{
	ds18b20_write_byte(cmd);

	if (cmd == CONVERT_T_CMD) {
		while(HAL_GPIO_ReadPin(TXRX_PORT, TXRX_PIN) == GPIO_PIN_RESET) {
		/* wait for end of conversion */
		}
	}
}

/**
 * Sends the rom command.
 *
 * @param 	cmd: Number of the rom command
 * @return  None.
 */
void ds18b20_send_rom_cmd(uint8_t cmd)
{
	ds18b20_write_byte(cmd);
}

/**
 * Read the temperature.
 *
 * @param 	None.
 * @return  Float value of the last measured temperature.
 */
float ds18b20_read_temp(void)
{
	int8_t k;
	uint8_t temp_LSB, temp_MSB;
	uint16_t u16_temp, mask = 1;
	float temperature = 0.0;

	temp_LSB = ds18b20_read_byte();
	temp_MSB = ds18b20_read_byte();

	u16_temp = ((temp_MSB << 8) | temp_LSB);

	for (k = -4; k < 7; k++) {
		if (u16_temp & mask) {
			temperature += powf(2,k);
		}
		mask = mask << 1;
	}
	return temperature;
}

/**
 * Write byte to DS18B20.
 *
 * @param 	data: Data to be written.
 * @return  None.
 */
void ds18b20_write_byte(uint8_t data)
{
	uint8_t i, mask = 0b00000001;
	uint8_t data_bit = data & mask;
	for (i = 0; i < 8; i++) {
		if (data_bit) {
			HAL_GPIO_WritePin(TXRX_PORT, TXRX_PIN, GPIO_PIN_RESET);
			delay_us(3);
			HAL_GPIO_WritePin(TXRX_PORT, TXRX_PIN, GPIO_PIN_SET);
			delay_us(90);
		} else {
			HAL_GPIO_WritePin(TXRX_PORT, TXRX_PIN, GPIO_PIN_RESET);
			delay_us(90);
			HAL_GPIO_WritePin(TXRX_PORT, TXRX_PIN, GPIO_PIN_SET);
			delay_us(3);
		}
		mask = mask << 1;
		data_bit = data & mask;
	}
}

/**
 * Read one bit from DS18B20.
 *
 * @param 	None.
 * @return  1 if succeed, 0 if failed.
 */
uint8_t ds18b20_read_bit(void)
{
	HAL_GPIO_WritePin(TXRX_PORT, TXRX_PIN, GPIO_PIN_RESET);
	delay_us(2);
	HAL_GPIO_WritePin(TXRX_PORT, TXRX_PIN, GPIO_PIN_SET);
	delay_us(10);
	if (HAL_GPIO_ReadPin(TXRX_PORT, TXRX_PIN) == GPIO_PIN_RESET) {
		delay_us(58);
		return 0;
	} else {
		delay_us(58);
		return 1;
	}
}

/**
 * Read one byte from DS18B20.
 *
 * @param 	None.
 * @return  One byte of data read from DS18B20.
 */
uint8_t ds18b20_read_byte(void)
{
	uint8_t i, data = 0;
	for (i = 0; i < 8; i++)
		data |= (ds18b20_read_bit() << i);
	return data;
}
