################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/DS3231/ds3231.c 

OBJS += \
./Drivers/BSP/DS3231/ds3231.o 

C_DEPS += \
./Drivers/BSP/DS3231/ds3231.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/DS3231/%.o Drivers/BSP/DS3231/%.su Drivers/BSP/DS3231/%.cyclo: ../Drivers/BSP/DS3231/%.c Drivers/BSP/DS3231/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/OLED -I../Drivers/BSP/LED -I../Drivers/BSP/KEY -I../APP/games -I../APP -I../Drivers/BSP/DS3231 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-DS3231

clean-Drivers-2f-BSP-2f-DS3231:
	-$(RM) ./Drivers/BSP/DS3231/ds3231.cyclo ./Drivers/BSP/DS3231/ds3231.d ./Drivers/BSP/DS3231/ds3231.o ./Drivers/BSP/DS3231/ds3231.su

.PHONY: clean-Drivers-2f-BSP-2f-DS3231

