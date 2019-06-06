################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/fat/sdcard_api.c \
../Source/fat/sdcard_spi.c \
../Source/fat/soft_spi.c 

OBJS += \
./Source/fat/sdcard_api.o \
./Source/fat/sdcard_spi.o \
./Source/fat/soft_spi.o 

C_DEPS += \
./Source/fat/sdcard_api.d \
./Source/fat/sdcard_spi.d \
./Source/fat/soft_spi.d 


# Each subdirectory must supply rules for building sources it contributes
Source/fat/%.o: ../Source/fat/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega128 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


