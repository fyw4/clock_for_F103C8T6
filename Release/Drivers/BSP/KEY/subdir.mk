################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/KEY/key.c 

OBJS += \
./Drivers/BSP/KEY/key.o 

C_DEPS += \
./Drivers/BSP/KEY/key.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/KEY/key.o: ../Drivers/BSP/KEY/key.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/KEY/key.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

