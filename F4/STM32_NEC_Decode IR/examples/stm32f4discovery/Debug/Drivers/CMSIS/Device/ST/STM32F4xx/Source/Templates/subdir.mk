################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.c 

OBJS += \
./Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.o 

C_DEPS += \
./Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/%.o: ../Drivers/CMSIS/Device/ST/STM32F4xx/Source/Templates/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DSTM32F407xx -I"/home/peter/Programming/EclipseWorkspace/NEC_Decode/Inc" -I"/home/peter/Programming/EclipseWorkspace/NEC_Decode/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/home/peter/Programming/EclipseWorkspace/NEC_Decode/Drivers/CMSIS/Include" -I"/home/peter/Programming/EclipseWorkspace/NEC_Decode/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -O0 -g3 -Wall -Wextra -c -mcpu=cortex-m4 -mthumb -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


