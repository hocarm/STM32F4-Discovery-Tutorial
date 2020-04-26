#include "ds1307.h"

#define ADDRESS 0xD0

#define TO_BCD(x) ( ((x / 10) << 4) | (x % 10) )
#define FROM_BCD(x) ( (x & 0x0F) + (x >> 4)*10 )

static uint8_t read_reg(ds1307 *handle, uint8_t reg) {
    uint8_t data = reg;
    HAL_I2C_Master_Transmit(handle->i2c, ADDRESS, &data, 1, 100);
    HAL_I2C_Master_Receive(handle->i2c, ADDRESS, &data, 1, 100);
    return data;
}

static void write_reg(ds1307 *handle, uint8_t reg, uint8_t value) {
    uint8_t data[2] = {reg, value};
    HAL_I2C_Master_Transmit(handle->i2c, ADDRESS, data, 2, 100);
}

void ds1307_init(ds1307 *handle, I2C_HandleTypeDef *i2c){
    handle->i2c = i2c;

    // Clear clock halt bit in register 0
    uint8_t reg = read_reg(handle, 0);
    reg &= 0x7F;
    write_reg(handle, 0, reg);

    // Select 24 hour mode in register 2
    reg = read_reg(handle, 2);
    reg &= 0xBF;
    write_reg(handle, 2, reg);
}

void ds1307_deinit(ds1307 *handle){

}

ds1307_datetime ds1307_get_datetime(ds1307 *handle){
    ds1307_datetime datetime;
    uint8_t data;

    data = read_reg(handle, 0) & 0x7F;
    datetime.seconds = FROM_BCD(data);

    data = read_reg(handle, 1);
    datetime.minutes = FROM_BCD(data);

    data = read_reg(handle, 2) & 0x3F;
    datetime.hours = FROM_BCD(data);

    data = read_reg(handle, 3);
    datetime.day = data & 0x07;

    data = read_reg(handle, 4);
    datetime.date = FROM_BCD(data);

    data = read_reg(handle, 5);
    datetime.month = FROM_BCD(data);

    data = read_reg(handle, 6);
    datetime.year = FROM_BCD(data) + 2000;

    return datetime;
}

void ds1307_set_datetime(ds1307 *handle, ds1307_datetime datetime){
    if (datetime.year >= 2000) datetime.year -= 2000;

    write_reg(handle, 0, TO_BCD(datetime.seconds) & 0x7F);
    write_reg(handle, 1, TO_BCD(datetime.minutes) & 0x7F);
    write_reg(handle, 2, TO_BCD(datetime.hours) & 0x3F);
    write_reg(handle, 3, datetime.day);
    write_reg(handle, 4, TO_BCD(datetime.date) & 0x3F);
    write_reg(handle, 5, TO_BCD(datetime.month) & 0x1F);
    write_reg(handle, 6, TO_BCD(datetime.year));
}
