################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../synergy/ssp/src/bsp/board/s7g2_dk/bsp_init.c \
../synergy/ssp/src/bsp/board/s7g2_dk/bsp_leds.c \
../synergy/ssp/src/bsp/board/s7g2_dk/bsp_qspi.c \
../synergy/ssp/src/bsp/board/s7g2_dk/bsp_sdram.c 

OBJS += \
./synergy/ssp/src/bsp/board/s7g2_dk/bsp_init.o \
./synergy/ssp/src/bsp/board/s7g2_dk/bsp_leds.o \
./synergy/ssp/src/bsp/board/s7g2_dk/bsp_qspi.o \
./synergy/ssp/src/bsp/board/s7g2_dk/bsp_sdram.o 

C_DEPS += \
./synergy/ssp/src/bsp/board/s7g2_dk/bsp_init.d \
./synergy/ssp/src/bsp/board/s7g2_dk/bsp_leds.d \
./synergy/ssp/src/bsp/board/s7g2_dk/bsp_qspi.d \
./synergy/ssp/src/bsp/board/s7g2_dk/bsp_sdram.d 


# Each subdirectory must supply rules for building sources it contributes
synergy/ssp/src/bsp/board/s7g2_dk/%.o: ../synergy/ssp/src/bsp/board/s7g2_dk/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	C:\Renesas\e2_studio\eclipse\../Utilities/isdebuild arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -I"C:\Users\lycan\e2_studio\workspace\DayOne\synergy_cfg\ssp_cfg\bsp" -I"C:\Users\lycan\e2_studio\workspace\DayOne\synergy_cfg\ssp_cfg\driver" -I"C:\Users\lycan\e2_studio\workspace\DayOne\synergy\ssp\inc" -I"C:\Users\lycan\e2_studio\workspace\DayOne\synergy\ssp\inc\bsp" -I"C:\Users\lycan\e2_studio\workspace\DayOne\synergy\ssp\inc\bsp\cmsis\Include" -I"C:\Users\lycan\e2_studio\workspace\DayOne\synergy\ssp\inc\driver\api" -I"C:\Users\lycan\e2_studio\workspace\DayOne\synergy\ssp\inc\driver\instances" -I"C:\Users\lycan\e2_studio\workspace\DayOne\src" -I"C:\Users\lycan\e2_studio\workspace\DayOne\src\synergy_gen" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<"
	@echo 'Finished building: $<'
	@echo ' '


