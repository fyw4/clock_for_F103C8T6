################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
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
APP/games/%.o APP/games/%.su APP/games/%.cyclo: ../APP/games/%.c APP/games/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/OLED -I../Drivers/BSP/LED -I../Drivers/BSP/KEY -I../APP/games -I../APP -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-APP-2f-games

clean-APP-2f-games:
	-$(RM) ./APP/games/Dino.cyclo ./APP/games/Dino.d ./APP/games/Dino.o ./APP/games/Dino.su ./APP/games/flappy_bird.cyclo ./APP/games/flappy_bird.d ./APP/games/flappy_bird.o ./APP/games/flappy_bird.su ./APP/games/start_war.cyclo ./APP/games/start_war.d ./APP/games/start_war.o ./APP/games/start_war.su

.PHONY: clean-APP-2f-games

