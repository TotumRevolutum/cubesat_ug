################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Source/MQ2/BaseMQ.c \
../Source/MQ2/adc.c \
../Source/MQ2/mq135.c \
../Source/MQ2/mq2.c 

OBJS += \
./Source/MQ2/BaseMQ.o \
./Source/MQ2/adc.o \
./Source/MQ2/mq135.o \
./Source/MQ2/mq2.o 

C_DEPS += \
./Source/MQ2/BaseMQ.d \
./Source/MQ2/adc.d \
./Source/MQ2/mq135.d \
./Source/MQ2/mq2.d 


# Each subdirectory must supply rules for building sources it contributes
Source/MQ2/%.o: ../Source/MQ2/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega128 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


