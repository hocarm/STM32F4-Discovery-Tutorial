# ds18b20_mflib
Basic library for handling the DS18B20 digital thermometer with STM32 HAL.

## Brief introduction to the library
This library should be used along with the STM32 HAL drivers. Nevertheless, after a few changes (mainly in the PIO control functions) it can be used with the StdPeriph library. Its current state should be called hard-beta if I can say so, and if manage to find some time I'll continue this work.

## How to use it?
Just copy the .h and .c file to your workspace directory. Read the description of the .c file. If you get an error concerning linking math.h module, add to your linker libraries the -lm parameter. 
