################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../game.c \
../serialize.c \
../spi.c \
../usart.c 

OBJS += \
./game.o \
./serialize.o \
./spi.o \
./usart.o 

C_DEPS += \
./game.d \
./serialize.d \
./spi.d \
./usart.d 


# Each subdirectory must supply rules for building sources it contributes
game.o: ../game.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 -I/home/kasper/SimplicityStudio/SDKs/gecko_sdk/util/third_party/fatfs/inc -I/home/kasper/SimplicityStudio/SDKs/gecko_sdk/hardware/kit/common/drivers -O2 -Wall -ffunction-sections -fdata-sections -c -fmessage-length=0 -MMD -MP -MF"game.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

serialize.o: ../serialize.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 -I/home/kasper/SimplicityStudio/SDKs/gecko_sdk/util/third_party/fatfs/inc -I/home/kasper/SimplicityStudio/SDKs/gecko_sdk/hardware/kit/common/drivers -O2 -Wall -ffunction-sections -fdata-sections -c -fmessage-length=0 -MMD -MP -MF"serialize.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

spi.o: ../spi.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 -I/home/kasper/SimplicityStudio/SDKs/gecko_sdk/util/third_party/fatfs/inc -I/home/kasper/SimplicityStudio/SDKs/gecko_sdk/hardware/kit/common/drivers -O2 -Wall -ffunction-sections -fdata-sections -c -fmessage-length=0 -MMD -MP -MF"spi.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

usart.o: ../usart.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 -I/home/kasper/SimplicityStudio/SDKs/gecko_sdk/util/third_party/fatfs/inc -I/home/kasper/SimplicityStudio/SDKs/gecko_sdk/hardware/kit/common/drivers -O2 -Wall -ffunction-sections -fdata-sections -c -fmessage-length=0 -MMD -MP -MF"usart.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


