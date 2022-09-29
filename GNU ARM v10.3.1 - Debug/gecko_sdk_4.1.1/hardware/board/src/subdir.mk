################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/kasper/SimplicityStudio/SDKs/gecko_sdk/hardware/board/src/sl_board_control_gpio.c \
/home/kasper/SimplicityStudio/SDKs/gecko_sdk/hardware/board/src/sl_board_init.c 

OBJS += \
./gecko_sdk_4.1.1/hardware/board/src/sl_board_control_gpio.o \
./gecko_sdk_4.1.1/hardware/board/src/sl_board_init.o 

C_DEPS += \
./gecko_sdk_4.1.1/hardware/board/src/sl_board_control_gpio.d \
./gecko_sdk_4.1.1/hardware/board/src/sl_board_init.d 


# Each subdirectory must supply rules for building sources it contributes
gecko_sdk_4.1.1/hardware/board/src/sl_board_control_gpio.o: /home/kasper/SimplicityStudio/SDKs/gecko_sdk/hardware/board/src/sl_board_control_gpio.c gecko_sdk_4.1.1/hardware/board/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DDEBUG_EFM=1' '-DEFM32GG990F1024=1' '-DSL_BOARD_NAME="BRD2200A"' '-DSL_BOARD_REV="B05"' '-DSL_COMPONENT_CATALOG_PRESENT=1' -I"/home/kasper/SimplicityStudio/v5_workspace/mcu/config" -I"/home/kasper/SimplicityStudio/v5_workspace/mcu/autogen" -I"/home/kasper/SimplicityStudio/v5_workspace/mcu" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/Device/SiliconLabs/EFM32GG/Include" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//hardware/board/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/CMSIS/Core/Include" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/service/device_init/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/emlib/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/common/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/common/toolchain/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/service/system/inc" -Os -Wall -Wextra -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -c -fmessage-length=0 -MMD -MP -MF"gecko_sdk_4.1.1/hardware/board/src/sl_board_control_gpio.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gecko_sdk_4.1.1/hardware/board/src/sl_board_init.o: /home/kasper/SimplicityStudio/SDKs/gecko_sdk/hardware/board/src/sl_board_init.c gecko_sdk_4.1.1/hardware/board/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g3 -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DDEBUG_EFM=1' '-DEFM32GG990F1024=1' '-DSL_BOARD_NAME="BRD2200A"' '-DSL_BOARD_REV="B05"' '-DSL_COMPONENT_CATALOG_PRESENT=1' -I"/home/kasper/SimplicityStudio/v5_workspace/mcu/config" -I"/home/kasper/SimplicityStudio/v5_workspace/mcu/autogen" -I"/home/kasper/SimplicityStudio/v5_workspace/mcu" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/Device/SiliconLabs/EFM32GG/Include" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//hardware/board/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/CMSIS/Core/Include" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/service/device_init/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/emlib/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/common/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/common/toolchain/inc" -I"/home/kasper/SimplicityStudio/SDKs/gecko_sdk//platform/service/system/inc" -Os -Wall -Wextra -ffunction-sections -fdata-sections -imacrossl_gcc_preinclude.h -c -fmessage-length=0 -MMD -MP -MF"gecko_sdk_4.1.1/hardware/board/src/sl_board_init.d" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


