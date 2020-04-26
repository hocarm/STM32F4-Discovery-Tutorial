#ifndef __DHT11_H__
#define __DHT11_H__
#include "stdbool.h"
//#include "stdio.h"

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_misc.h"

typedef enum {
    DHT11_NONE = 0,
    DHT11_INIT_PULL_DOWN  =1,
    DHT11_INIT_RELEASE = 2,
    DHT11_PULL_DOWN = 3,
    DHT11_RELEASE_1 = 4,
    DHT11_RELEASE_0 = 5,
    DHT11_EOT,
} dht11_state;

void dht11_init(GPIO_TypeDef *gpio_, u16 pin_, u8 source_pin_, TIM_TypeDef *timer_);
void dht11_trigger_state_machine(u32 timer, u8 bit);
bool dht11_reset_pulse();
//bool dht11_check_tolerance(u32 timer);
void dht11_decode_data();
u8 dht11_get_temperature();
u8 dht11_get_rh();
#endif //__DHT11_H__
