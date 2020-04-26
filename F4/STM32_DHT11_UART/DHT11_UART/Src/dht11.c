#include "dht11.h"

GPIO_TypeDef *gpio;
u16 pin;
TIM_TypeDef *timer;
u8 source_port;
u8 source_pin;
u32 interrupt_line;

dht11_state dht11_current_state = DHT11_NONE;
u16 dht11_pulse_lengths[6];

bool dht11_data[40];
u8 data_pos = 0;

u8 last_temperature = 0;
u8 last_relative_humidity = 0;

static void dht11_delay_us(unsigned int time) {
	timer->CNT = 0;
	time -= 3;
	while (timer->CNT <= time) {}
}

// Configures GPIO pin, timer and interrupts
void dht11_init(GPIO_TypeDef *gpio_, u16 pin_, u8 source_pin_, TIM_TypeDef *timer_) {
    gpio = gpio_;
    pin = pin_;
    timer = timer_;
    source_pin = source_pin_;
    interrupt_line = pin_;

    // Time in us
    dht11_pulse_lengths[DHT11_NONE] = 0;
    dht11_pulse_lengths[DHT11_INIT_PULL_DOWN] = 80;
    dht11_pulse_lengths[DHT11_INIT_RELEASE] = 80;
    dht11_pulse_lengths[DHT11_PULL_DOWN] = 50;
    dht11_pulse_lengths[DHT11_RELEASE_0] = 25;
    dht11_pulse_lengths[DHT11_RELEASE_1] = 70;
    dht11_pulse_lengths[DHT11_EOT] = 50;

    // Enable GPIO clock
    if (gpio == GPIOA) {
        source_port = GPIO_PortSourceGPIOA;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    } else if (gpio == GPIOB) {
        source_port = GPIO_PortSourceGPIOB;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    } else if (gpio == GPIOC) {
        source_port = GPIO_PortSourceGPIOC;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    } else {
        while(1) {}
        //TODO: not implemented
    }

    // Configure pin
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(gpio, &GPIO_InitStructure);

    // Enable interrupts for pin
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_EXTILineConfig(source_port, source_pin);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = interrupt_line;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // Enable Timer clock
    if (timer == TIM2) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    } else if (timer == TIM3) {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    } else {
        // TODO: not implemented
        while(1){}
    }

    // Configure timer
    TIM_TimeBaseInitTypeDef TIM_InitStructure;
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStructure.TIM_Prescaler = SystemCoreClock / 1000000 - 1;
    TIM_InitStructure.TIM_Period = 10000 - 1; // Update event every 10000 us (10 ms)
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(timer, &TIM_InitStructure);

    TIM_Cmd(timer, ENABLE);
}

// Sends reset pulse
bool dht11_reset_pulse() {
    // Pull bus down for at least 18 ms
    GPIO_ResetBits(gpio, pin);
    dht11_delay_us(5000);
    dht11_delay_us(5000);
    dht11_delay_us(5000);
    dht11_delay_us(5000);
    GPIO_SetBits(gpio, pin);

    // Wait for recovery (20-40 us)
    dht11_current_state = DHT11_NONE;
    TIM_SetCounter(timer, 0);
    dht11_delay_us(40);

    return false;
}


// Checks if received signal has expected length
bool dht11_check_tolerance(u32 timer) {

    u16 expected = dht11_pulse_lengths[dht11_current_state];

    if (dht11_current_state == DHT11_NONE) {
        return true;
    }

    u8 tolerance = 10; // 10%
    u32 min = expected - (tolerance / 10.0) * expected;
    u32 max = expected + (tolerance / 10.0) * expected;

    if ((timer >= min) && (timer <= max)) {
        return true;
    }
//    char tab[50];
//    sprintf(tab, "Error, expected: %d at position %d, received: %u %d\r\n", expected, data_pos, (unsigned int)timer, dht11_current_state);
//    printf(tab);
    return false;
}

void dht11_trigger_state_machine(u32 timer, u8 bit) {
    u8 middle_value = (dht11_pulse_lengths[DHT11_RELEASE_0] + dht11_pulse_lengths[DHT11_RELEASE_1]) / 2;
    dht11_check_tolerance(timer);

//    char tab[10];
//    sprintf(tab, "%u\r\n", (unsigned int)timer);

    switch(dht11_current_state) {
        case DHT11_NONE:
            if (bit) {
                break; // wrong state
            }
//            GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
            dht11_current_state = DHT11_INIT_PULL_DOWN;
//            GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
            break;
        case DHT11_INIT_PULL_DOWN:
            dht11_current_state = DHT11_INIT_RELEASE;
            break;
        case DHT11_INIT_RELEASE:
            dht11_current_state = DHT11_PULL_DOWN;
            break;
        case DHT11_PULL_DOWN:
            dht11_current_state = DHT11_RELEASE_1;
            break;


        case DHT11_RELEASE_0:
        case DHT11_RELEASE_1:
            if (timer < middle_value) {
                dht11_data[data_pos] = 0;
            } else {
                dht11_data[data_pos] = 1;
            }
            ++data_pos;

            dht11_current_state = DHT11_PULL_DOWN;

            if (data_pos == 40) {
                data_pos = 0;
                dht11_current_state = DHT11_EOT;
                dht11_decode_data();
            }

            break;

        case DHT11_EOT:
//            GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
            dht11_current_state = DHT11_NONE;
//            GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
            break;
        default:
            break;
    }
}

void dht11_decode_data() {
    u8 i;
//    char tab[43];
//    for (i = 0; i < 40; ++i) {
//        tab[i] = 48 + data[i];
//    }
//    tab[40] = '\r';
//    tab[41] = '\n';
//    tab[42] = 0;
//    printf(tab);

    u8 temp_integral = 0, temp_decimal = 0, rh_integral = 0, rh_decimal = 0, checksum = 0;
    for (i = 0; i < 8; ++i) {
        rh_integral     |= dht11_data[i]      << (7 - i);
        rh_decimal      |= dht11_data[8  + i] << (7 - i);
        temp_integral   |= dht11_data[16 + i] << (7 - i);
        temp_decimal    |= dht11_data[24 + i] << (7 - i);
        checksum        |= dht11_data[32 + i] << (7 - i);
    }
//    sprintf(tab, "%d.%d %d.%d %d\r\n", temp_integral, temp_decimal, rh_integral, rh_decimal, checksum);
    if (temp_integral + temp_decimal + rh_integral + rh_decimal == checksum) {
        last_temperature = temp_integral;
        last_relative_humidity = rh_integral;
    } else {
//        printf("Checksum incorrect\r\n");
        last_temperature = 0;
        last_relative_humidity = 0;
    }
//    printf(tab);
}

u8 dht11_get_temperature() {
    return last_temperature;
}

u8 dht11_get_rh() {
    return last_relative_humidity;
}

void EXTI9_5_IRQHandler(void) {
    static unsigned int counter;

    if (EXTI_GetITStatus(interrupt_line) != RESET) {
        if (dht11_current_state != DHT11_EOT) {
            u8 bit = GPIO_ReadInputDataBit(gpio, pin);
//            if (bit) {
//                    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
//            } else {
//                    GPIO_SetBits(GPIOC, GPIO_Pin_13);
//            }

            // Restart Timer
            counter = TIM_GetCounter(timer);
            TIM_SetCounter(timer, 0);
            dht11_trigger_state_machine(counter, bit);
        }

        EXTI_ClearITPendingBit(interrupt_line);
    }
}
