################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Source/ds18_h/subdir.mk
-include Source/MQ2/subdir.mk
-include Source/1wire_h/subdir.mk
-include Source/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
CubeSat_all.lss \

FLASH_IMAGE += \
CubeSat_all.hex \

EEPROM_IMAGE += \
CubeSat_all.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: CubeSat_all.elf secondary-outputs

# Tool invocations
CubeSat_all.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,CubeSat_all.map -Wl,-u,vfprintf -lprintf_flt -lm -mmcu=atmega128 -o "CubeSat_all.elf" $(OBJS) $(USER_OBJS) $(LIBS) -lc
	@echo 'Finished building target: $@'
	@echo ' '

CubeSat_all.lss: CubeSat_all.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S CubeSat_all.elf  >"CubeSat_all.lss"
	@echo 'Finished building: $@'
	@echo ' '

CubeSat_all.hex: CubeSat_all.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex CubeSat_all.elf  "CubeSat_all.hex"
	@echo 'Finished building: $@'
	@echo ' '

CubeSat_all.eep: CubeSat_all.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex CubeSat_all.elf  "CubeSat_all.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: CubeSat_all.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega128 CubeSat_all.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) CubeSat_all.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
