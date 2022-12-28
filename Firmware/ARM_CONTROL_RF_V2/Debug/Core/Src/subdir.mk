################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/MPU6050.c \
../Core/Src/app.c \
../Core/Src/crc16.c \
../Core/Src/hw.c \
../Core/Src/main.c \
../Core/Src/nrf24.c \
../Core/Src/nrf24_app.c \
../Core/Src/s_luxflex.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c \
../Core/Src/utility.c 

OBJS += \
./Core/Src/MPU6050.o \
./Core/Src/app.o \
./Core/Src/crc16.o \
./Core/Src/hw.o \
./Core/Src/main.o \
./Core/Src/nrf24.o \
./Core/Src/nrf24_app.o \
./Core/Src/s_luxflex.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o \
./Core/Src/utility.o 

C_DEPS += \
./Core/Src/MPU6050.d \
./Core/Src/app.d \
./Core/Src/crc16.d \
./Core/Src/hw.d \
./Core/Src/main.d \
./Core/Src/nrf24.d \
./Core/Src/nrf24_app.d \
./Core/Src/s_luxflex.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d \
./Core/Src/utility.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F103xB -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/MPU6050.d ./Core/Src/MPU6050.o ./Core/Src/MPU6050.su ./Core/Src/app.d ./Core/Src/app.o ./Core/Src/app.su ./Core/Src/crc16.d ./Core/Src/crc16.o ./Core/Src/crc16.su ./Core/Src/hw.d ./Core/Src/hw.o ./Core/Src/hw.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/nrf24.d ./Core/Src/nrf24.o ./Core/Src/nrf24.su ./Core/Src/nrf24_app.d ./Core/Src/nrf24_app.o ./Core/Src/nrf24_app.su ./Core/Src/s_luxflex.d ./Core/Src/s_luxflex.o ./Core/Src/s_luxflex.su ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_hal_msp.su ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/stm32f1xx_it.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o ./Core/Src/system_stm32f1xx.su ./Core/Src/utility.d ./Core/Src/utility.o ./Core/Src/utility.su

.PHONY: clean-Core-2f-Src

