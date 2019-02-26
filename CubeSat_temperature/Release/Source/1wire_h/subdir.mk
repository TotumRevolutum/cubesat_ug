################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/1wire_h/crc8.c \
../Source/1wire_h/onewire.c 

OBJS += \
./Source/1wire_h/crc8.o \
./Source/1wire_h/onewire.o 

C_DEPS += \
./Source/1wire_h/crc8.d \
./Source/1wire_h/onewire.d 


# Each subdirectory must supply rules for building sources it contributes
Source/1wire_h/%.o: ../Source/1wire_h/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega128 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


