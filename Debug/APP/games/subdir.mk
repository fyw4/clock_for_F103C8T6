################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../APP/games/Dino.c \
../APP/games/flappy_bird.c \
../APP/games/start_war.c 

OBJS += \
./APP/games/Dino.o \
./APP/games/flappy_bird.o \
./APP/games/start_war.o 

C_DEPS += \
./APP/games/Dino.d \
./APP/games/flappy_bird.d \
./APP/games/start_war.d 


# Each subdirectory must supply rules for building sources it contributes
APP/games/Dino.o: ../APP/games/Dino.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/OLED -I../Drivers/BSP/LED -I../Drivers/BSP/KEY -I../APP/games -I../APP -I../Drivers/BSP/DS3231 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APP/games/Dino.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
APP/games/flappy_bird.o: ../APP/games/flappy_bird.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/OLED -I../Drivers/BSP/LED -I../Drivers/BSP/KEY -I../APP/games -I../APP -I../Drivers/BSP/DS3231 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APP/games/flappy_bird.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
APP/games/start_war.o: ../APP/games/start_war.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/OLED -I../Drivers/BSP/LED -I../Drivers/BSP/KEY -I../APP/games -I../APP -I../Drivers/BSP/DS3231 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APP/games/start_war.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

