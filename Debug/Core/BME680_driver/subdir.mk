################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/BME680_driver/bme680.c 

OBJS += \
./Core/BME680_driver/bme680.o 

C_DEPS += \
./Core/BME680_driver/bme680.d 


# Each subdirectory must supply rules for building sources it contributes
Core/BME680_driver/%.o Core/BME680_driver/%.su: ../Core/BME680_driver/%.c Core/BME680_driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Oscar/Desktop/Final/Core/BME680_driver" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-BME680_driver

clean-Core-2f-BME680_driver:
	-$(RM) ./Core/BME680_driver/bme680.d ./Core/BME680_driver/bme680.o ./Core/BME680_driver/bme680.su

.PHONY: clean-Core-2f-BME680_driver

