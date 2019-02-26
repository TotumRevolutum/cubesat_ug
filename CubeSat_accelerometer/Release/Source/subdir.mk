################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/adxl345_i2c.c \
../Source/i2c_avr.c \
../Source/main.c \
../Source/uart.c 

OBJS += \
./Source/adxl345_i2c.o \
./Source/i2c_avr.o \
./Source/main.o \
./Source/uart.o 

C_DEPS += \
./Source/adxl345_i2c.d \
./Source/i2c_avr.d \
./Source/main.d \
./Source/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Source/%.o: ../Source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega128 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


