#include "stm32f1xx_hal.h"

typedef struct {
    I2C_HandleTypeDef *i2c;
} ds1307;

typedef enum {
    MONDAY = 1, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY
} ds1307_day;

typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    ds1307_day day;
    uint8_t date;
    uint8_t month;
    uint16_t year;
} ds1307_datetime;

void ds1307_init(ds1307 *handle, I2C_HandleTypeDef *i2c);

void ds1307_deinit(ds1307 *handle);

ds1307_datetime ds1307_get_datetime(ds1307 *handle);

void ds1307_set_datetime(ds1307 *handle, ds1307_datetime datetime);
