#ifndef DS1307_H_
#define DS1307_H_

#include "stm32f4xx_hal.h"

typedef struct DS1307_Handle {
	I2C_HandleTypeDef 		*i2c;
	uint8_t 				DS1307_ADDRESS;
	uint32_t 				DS1307_CLOCK;
	I2C_TypeDef 			*DS1307_I2Cx;
	uint32_t 				TIMEOUT;
} DS1307_Handle;

typedef enum {
	DS1307_RES_OK=0,
	DS1307_RES_TIMEOUT=1,
	DS1307_RES_ERROR=2
} DS1307_RESULT;

typedef enum {
	DS1307_RATE_1HZ=0,
	DS1307_RATE_4096HZ=1,
	DS1307_RATE_8192HZ=2,
	DS1307_RATE_32768HZ=3
} DS1307_RATE;

typedef enum {
	DS1307_DAY_SUNDAY=1,
	DS1307_DAY_MONDAY=2,
	DS1307_DAY_TUESDAY=3,
	DS1307_DAY_WEDNESDAY=4,
	DS1307_DAY_THURSDAY=5,
	DS1307_DAY_FRIDAY=6,
	DS1307_DAY_SATURDAY=7
} DS1307_DAY;

typedef struct DS1307_TIME{
	uint16_t year;
	uint8_t month;
	uint8_t date;
	DS1307_DAY day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
} DS1307_TIME;

extern void DS1307_Error(void);

DS1307_RESULT DS1307_Init(DS1307_Handle* handle);
DS1307_TIME DS1307_GetTime(DS1307_Handle* handle);
DS1307_RESULT DS1307_SetTime(DS1307_Handle* handle,DS1307_TIME time);
DS1307_RESULT DS1307_EnableSquareWave(DS1307_Handle* handle);
DS1307_RESULT DS1307_DisableSquareWave(DS1307_Handle* handle);
DS1307_RESULT DS1307_SelectRate(DS1307_Handle* handle,DS1307_RATE rate);
DS1307_RESULT DS1307_ClockHalt(DS1307_Handle* handle);
DS1307_RESULT DS1307_ClockResume(DS1307_Handle* handle);
DS1307_RESULT DS1307_WaitUntilReady(DS1307_Handle* handle);

#endif /* DS1307_H_ */
