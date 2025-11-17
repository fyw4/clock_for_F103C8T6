################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
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
Drivers/BSP/OLED/%.o Drivers/BSP/OLED/%.su Drivers/BSP/OLED/%.cyclo: ../Drivers/BSP/OLED/%.c Drivers/BSP/OLED/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/OLED -I../Drivers/BSP/LED -I../Drivers/BSP/KEY -I../APP -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-OLED

clean-Drivers-2f-BSP-2f-OLED:
	-$(RM) ./Drivers/BSP/OLED/font.cyclo ./Drivers/BSP/OLED/font.d ./Drivers/BSP/OLED/font.o ./Drivers/BSP/OLED/font.su ./Drivers/BSP/OLED/oled.cyclo ./Drivers/BSP/OLED/oled.d ./Drivers/BSP/OLED/oled.o ./Drivers/BSP/OLED/oled.su

.PHONY: clean-Drivers-2f-BSP-2f-OLED

