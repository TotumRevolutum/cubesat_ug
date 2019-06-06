################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/DataProcessing.c \
../Source/DataTransmit.c \
../Source/adxl345_i2c.c \
../Source/bmp280_i2c.c \
../Source/i2c_avr.c \
../Source/main.c \
../Source/sdcard_raw.c \
../Source/uart.c 

OBJS += \
./Source/DataProcessing.o \
./Source/DataTransmit.o \
./Source/adxl345_i2c.o \
./Source/bmp280_i2c.o \
./Source/i2c_avr.o \
./Source/main.o \
./Source/sdcard_raw.o \
./Source/uart.o 

C_DEPS += \
./Source/DataProcessing.d \
./Source/DataTransmit.d \
./Source/adxl345_i2c.d \
./Source/bmp280_i2c.d \
./Source/i2c_avr.d \
./Source/main.d \
./Source/sdcard_raw.d \
./Source/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Source/%.o: ../Source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega128 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


