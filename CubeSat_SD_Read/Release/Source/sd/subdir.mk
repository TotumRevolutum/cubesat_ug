################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/sd/sdcard_api.c \
../Source/sd/sdcard_spi.c \
../Source/sd/soft_spi.c \
../Source/sd/uart_text_io.c 

OBJS += \
./Source/sd/sdcard_api.o \
./Source/sd/sdcard_spi.o \
./Source/sd/soft_spi.o \
./Source/sd/uart_text_io.o 

C_DEPS += \
./Source/sd/sdcard_api.d \
./Source/sd/sdcard_spi.d \
./Source/sd/soft_spi.d \
./Source/sd/uart_text_io.d 


# Each subdirectory must supply rules for building sources it contributes
Source/sd/%.o: ../Source/sd/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega128 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


