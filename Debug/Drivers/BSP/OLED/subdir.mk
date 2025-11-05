################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/OLED/font.c \
../Drivers/BSP/OLED/oled.c 

OBJS += \
./Drivers/BSP/OLED/font.o \
./Drivers/BSP/OLED/oled.o 

C_DEPS += \
./Drivers/BSP/OLED/font.d \
./Drivers/BSP/OLED/oled.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/OLED/font.o: ../Drivers/BSP/OLED/font.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/OLED -I../Drivers/BSP/LED -I../Drivers/BSP/KEY -I../APP -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/OLED/font.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
Drivers/BSP/OLED/oled.o: ../Drivers/BSP/OLED/oled.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/OLED -I../Drivers/BSP/LED -I../Drivers/BSP/KEY -I../APP -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/OLED/oled.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

