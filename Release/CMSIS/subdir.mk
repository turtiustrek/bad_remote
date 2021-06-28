################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/system_MKL16Z4.c 

OBJS += \
./CMSIS/system_MKL16Z4.o 

C_DEPS += \
./CMSIS/system_MKL16Z4.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/%.o: ../CMSIS/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MKL16Z64VLH4 -DCPU_MKL16Z64VLH4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DNDEBUG -D__NEWLIB__ -I"/home/apple/Programming/MCUXpresso/bad_remote/drivers" -I"/home/apple/Programming/MCUXpresso/bad_remote/utilities" -I"/home/apple/Programming/MCUXpresso/bad_remote/CMSIS" -I"/home/apple/Programming/MCUXpresso/bad_remote/board" -I"/home/apple/Programming/MCUXpresso/bad_remote/source" -I"/home/apple/Programming/MCUXpresso/bad_remote" -I"/home/apple/Programming/MCUXpresso/bad_remote/CMSIS" -I"/home/apple/Programming/MCUXpresso/bad_remote/drivers" -I"/home/apple/Programming/MCUXpresso/bad_remote/utilities" -I"/home/apple/Programming/MCUXpresso/bad_remote/startup" -Os -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


