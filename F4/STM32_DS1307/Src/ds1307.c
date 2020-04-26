#include "ds1307.h"

/*	DS1307_Init handles all the initialization
 * 	routines neccessary for the proper functioning
 * 	of the device. Call this function always
 * 	before you try to use any other functions
 * 	from this library.
 *
 * 	PARS: uint8_t init_i2c: if you want this routine to also
 * 							initialize the I2C bus = 1
 *
 * 							else = 0
 * 	RETVAL: none
 * */
DS1307_RESULT DS1307_Init(DS1307_Handle* handle) {

	handle->i2c->Instance = handle->DS1307_I2Cx;
	handle->i2c->Init.ClockSpeed = handle->DS1307_CLOCK;

	handle->i2c->Init.DutyCycle = I2C_DUTYCYCLE_2;
	handle->i2c->Init.OwnAddress1 = 0;
	handle->i2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	handle->i2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
	handle->i2c->Init.OwnAddress2 = 0;
	handle->i2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
	handle->i2c->Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;

	HAL_I2C_MspInit(handle->i2c);
	HAL_I2C_Init(handle->i2c);
	return DS1307_WaitUntilReady(handle);
}

/*	DS1307_getControlRegister returns the current state
 * 	of the control register
 *
 * 	PARS: none
 * 	RETVAL: uint8_t
 * */
uint8_t DS1307_getControlRegister(DS1307_Handle* handle) {

	/*
	 if (DS1307_start(I2C_Direction_Transmitter) != 0) {
	 DS1307_Error();
	 }

	 if (DS1307_write(7) != 0) {
	 DS1307_Error();
	 }

	 if (DS1307_start(I2C_Direction_Receiver) != 0) {
	 DS1307_Error();
	 }
	 return DS1307_read_nack();
	 */

	if (DS1307_WaitUntilReady(handle) == DS1307_RES_OK) {
		uint8_t a = 7;
		if (HAL_I2C_Master_Transmit(handle->i2c, handle->DS1307_ADDRESS, &a, 1,
				handle->TIMEOUT)) {
			return 0;
		}

		if (HAL_I2C_Master_Receive(handle->i2c, handle->DS1307_ADDRESS, &a, 1,
				handle->TIMEOUT)) {
			return 0;
		}
		DS1307_WaitUntilReady(handle);
		return a;
	} else {
		return 0;
	}
}

/*	DS1307_setControlRegister sets the value of the
 * 	control register to the desired value
 *
 * 	PARS: uint8_t reg: represent the desired value
 * 						of the control register
 *
 * 	RETVAL: none
 */

DS1307_RESULT DS1307_setControlRegister(DS1307_Handle* handle, uint8_t reg) {

	if (DS1307_WaitUntilReady(handle) == DS1307_RES_OK) {
		uint8_t a[2] = { 7, reg };
		if (HAL_I2C_Master_Transmit(handle->i2c, handle->DS1307_ADDRESS, a, 2,
				handle->TIMEOUT)) {
			return DS1307_RES_ERROR;
		}
	} else {
		return DS1307_RES_TIMEOUT;
	}
	return DS1307_WaitUntilReady(handle);

}

/*	DS1307_EnableSquareWave enables the square wave
 * 	output functionality of the DS1307
 *
 * 	PARS: none
 * 	RETVAL: none
 */

DS1307_RESULT DS1307_EnableSquareWave(DS1307_Handle* handle) {
	if (DS1307_WaitUntilReady(handle) == DS1307_RES_OK) {
		uint8_t a = DS1307_getControlRegister(handle);
		a |= 1 << 4;
		DS1307_setControlRegister(handle, a);
	} else {
		return DS1307_RES_TIMEOUT;
	}
	return DS1307_WaitUntilReady(handle);
}

/*	DS1307_DisableSquareWave disables the square wave
 * 	output functionality of the DS1307
 *
 * 	PARS: none
 * 	RETVAL: none
 */

DS1307_RESULT DS1307_DisableSquareWave(DS1307_Handle* handle) {
	if (DS1307_WaitUntilReady(handle) == DS1307_RES_OK) {
		uint8_t a = DS1307_getControlRegister(handle);
		a &= ~(1 << 4);
		DS1307_setControlRegister(handle, a);
	} else {
		return DS1307_RES_TIMEOUT;
	}
	return DS1307_WaitUntilReady(handle);
}

/*	DS1307_SelectRate sets the frequency of the
 * 	square wave output
 *
 * 	PARS: uint8_t rate: DS1307_RATE_1HZ
 * 						DS1307_RATE_4096HZ
 * 						DS1307_RATE_8192HZ
 * 						DS1307_RATE_32768HZ
 * 	RETVAL: none
 */

DS1307_RESULT DS1307_SelectRate(DS1307_Handle* handle, DS1307_RATE rate) {
	if (DS1307_WaitUntilReady(handle) == DS1307_RES_OK) {
		uint8_t a = DS1307_getControlRegister(handle);
		a &= 0xFC;
		rate &= 3;
		a |= rate;
		DS1307_setControlRegister(handle, a);
	} else {
		return DS1307_RES_TIMEOUT;
	}
	return DS1307_WaitUntilReady(handle);
}

/*	DS1307_GetTime return the current time stored
 * 	in the DS1307 RTC
 *
 * 	PARS: none
 * 	RETVAL: DS1307_TIME: represents the current time
 */

DS1307_TIME DS1307_GetTime(DS1307_Handle* handle) {

	DS1307_TIME time;
	if (DS1307_WaitUntilReady(handle) == DS1307_RES_OK) {



		time.year = 0;
		time.month = 0;
		time.date = 0;
		time.day = 0;
		time.hour = 0;
		time.minute = 0;
		time.second = 0;

		uint8_t data[8];

		uint8_t a = 0;
		if (HAL_I2C_Master_Transmit(handle->i2c, handle->DS1307_ADDRESS, &a, 1,
				handle->TIMEOUT)) {
			return time;
		}
		if (HAL_I2C_Master_Receive(handle->i2c, handle->DS1307_ADDRESS, data, 8,
				handle->TIMEOUT)) {
			return time;
		}

		time.second += data[0] & 0xF;
		time.second += 10 * ((data[0] & 0x70) >> 4);

		time.minute += data[1] & 0xF;
		time.minute += 10 * ((data[1] & 0x70) >> 4);

		if (data[2] & (1 << 6)) {	//12H

		} else {	//24H
			time.hour += data[2] & 0xF;
			time.hour += 10 * ((data[2] & 0x30) >> 4);
		}

		time.day += data[3] & 0x7;

		time.date += data[4] & 0xF;
		time.date += 10 * ((data[4] & 0x30) >> 4);

		time.month += data[5] & 0xF;
		time.month += 10 * ((data[5] & 0x10) >> 4);

		time.year += 2000 + (data[6] & 0xF);
		time.year += 10 * ((data[6] & 0xF0) >> 4);
		DS1307_WaitUntilReady(handle);
		return time;
	}
	return time;
}

/*	DS1307_SetTime programs the current time into the RTC
 *
 * 	PARS: DS1307_TIME time: the time you want to store in RTC
 * 	RETVAL: none
 */

DS1307_RESULT DS1307_SetTime(DS1307_Handle* handle, DS1307_TIME time) {

	if (DS1307_WaitUntilReady(handle) == DS1307_RES_OK) {
		uint8_t data[8];

		for (int i = 0; i < 8; i++) {
			data[i] = 0;
		}

		data[1] |= (time.second % 10);
		data[1] |= ((time.second - (time.second % 10)) / 10) << 4;
		data[1] &= ~(1 << 7);

		data[2] |= (time.minute % 10);
		data[2] |= ((time.minute - (time.minute % 10)) / 10) << 4;

		data[3] |= (time.hour % 10);
		data[3] |= ((time.hour - (time.hour % 10)) / 10) << 4;
		data[3] &= ~(1 << 6);

		data[4] |= time.day;

		data[5] |= (time.date % 10);
		data[5] |= ((time.date - (time.date % 10)) / 10) << 4;

		data[6] |= (time.month % 10);
		data[6] |= ((time.month - (time.month % 10)) / 10) << 4;

		data[7] |= ((time.year - 2000) % 10);
		data[7] |= (((time.year - 2000) - ((time.year - 2000) % 10)) / 10) << 4;

		if (HAL_I2C_Master_Transmit(handle->i2c, handle->DS1307_ADDRESS, data,
				8, handle->TIMEOUT)) {
			return DS1307_RES_ERROR;
		}
	} else {
		return DS1307_RES_TIMEOUT;
	}
	return DS1307_WaitUntilReady(handle);
}

DS1307_RESULT DS1307_ClockHalt(DS1307_Handle* handle) {

	if (DS1307_WaitUntilReady(handle) == DS1307_RES_OK) {
		uint8_t reg = 0;

		uint8_t a = 0;
		if (HAL_I2C_Master_Transmit(handle->i2c, handle->DS1307_ADDRESS, &a, 1,
				handle->TIMEOUT)) {
			return DS1307_RES_ERROR;
		}
		if (HAL_I2C_Master_Receive(handle->i2c, handle->DS1307_ADDRESS, &reg, 1,
				handle->TIMEOUT)) {
			return DS1307_RES_ERROR;
		}

		reg |= 1 << 7;
		uint8_t data[2] = { 0, reg };

		if (HAL_I2C_Master_Transmit(handle->i2c, handle->DS1307_ADDRESS, data,
				2, handle->TIMEOUT)) {
			return DS1307_RES_ERROR;
		}
	} else {
		return DS1307_RES_TIMEOUT;
	}
	return DS1307_WaitUntilReady(handle);

}

DS1307_RESULT DS1307_ClockResume(DS1307_Handle* handle) {

	if (DS1307_WaitUntilReady(handle) == DS1307_RES_OK) {
		uint8_t reg = 0;

		uint8_t a = 0;
		HAL_StatusTypeDef b = HAL_I2C_Master_Transmit(handle->i2c,
				handle->DS1307_ADDRESS, &a, 1, handle->TIMEOUT);
		if (b) {
			return DS1307_RES_ERROR;
		}
		if (HAL_I2C_Master_Receive(handle->i2c, handle->DS1307_ADDRESS, &reg, 1,
				handle->TIMEOUT)) {
			return DS1307_RES_ERROR;
		}

		reg &= ~(1 << 7);
		uint8_t data[2] = { 0, reg };

		if (HAL_I2C_Master_Transmit(handle->i2c, handle->DS1307_ADDRESS, data,
				2, handle->TIMEOUT)) {
			return DS1307_RES_ERROR;
		}
	} else {
		return DS1307_RES_TIMEOUT;
	}
	return DS1307_WaitUntilReady(handle);
}

DS1307_RESULT DS1307_WaitUntilReady(DS1307_Handle* handle) {
	//while(HAL_I2C_GetState(handle->i2c)!=HAL_I2C_STATE_READY);
	int time = SysTick->VAL;
	while ((HAL_I2C_GetState(handle->i2c) != HAL_I2C_STATE_READY)
			&& (time < handle->TIMEOUT + SysTick->VAL)) {
		time = SysTick->VAL;
	}
	if (HAL_I2C_GetState(handle->i2c) == HAL_I2C_STATE_READY) {
		return DS1307_RES_OK;
	} else {
		return DS1307_RES_TIMEOUT;
	}
}
