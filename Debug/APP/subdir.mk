################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../APP/clock.c \
../APP/info.c \
../APP/menu.c 

OBJS += \
./APP/clock.o \
./APP/info.o \
./APP/menu.o 

C_DEPS += \
./APP/clock.d \
./APP/info.d \
./APP/menu.d 


# Each subdirectory must supply rules for building sources it contributes
APP/clock.o: ../APP/clock.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/OLED -I../Drivers/BSP/LED -I../Drivers/BSP/KEY -I../APP -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APP/clock.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
APP/info.o: ../APP/info.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/OLED -I../Drivers/BSP/LED -I../Drivers/BSP/KEY -I../APP -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APP/info.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
APP/menu.o: ../APP/menu.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/OLED -I../Drivers/BSP/LED -I../Drivers/BSP/KEY -I../APP -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APP/menu.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

